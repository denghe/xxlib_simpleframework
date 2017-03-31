#include "UnityGeom.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstring>

// Returns a random number [0..1)
static float frand()
{
	//	return ((float)(rand() & 0xffff)/(float)0xffff);
	return (float)rand() / (float)RAND_MAX;
}

inline bool inRange(const float* v1, const float* v2, const float r, const float h)
{
	const float dx = v2[0] - v1[0];
	const float dy = v2[1] - v1[1];
	const float dz = v2[2] - v1[2];
	return (dx*dx + dz*dz) < r*r && fabsf(dy) < h;
}

static int fixupCorridor(dtPolyRef* path, const int npath, const int maxPath,
	const dtPolyRef* visited, const int nvisited)
{
	int furthestPath = -1;
	int furthestVisited = -1;

	// Find furthest common polygon.
	for (int i = npath - 1; i >= 0; --i)
	{
		bool found = false;
		for (int j = nvisited - 1; j >= 0; --j)
		{
			if (path[i] == visited[j])
			{
				furthestPath = i;
				furthestVisited = j;
				found = true;
			}
		}
		if (found)
			break;
	}

	// If no intersection found just return current path. 
	if (furthestPath == -1 || furthestVisited == -1)
		return npath;

	// Concatenate paths.	

	// Adjust beginning of the buffer to include the visited.
	const int req = nvisited - furthestVisited;
	const int orig = rcMin(furthestPath + 1, npath);
	int size = rcMax(0, npath - orig);
	if (req + size > maxPath)
		size = maxPath - req;
	if (size)
		memmove(path + req, path + orig, size * sizeof(dtPolyRef));

	// Store visited
	for (int i = 0; i < req; ++i)
		path[i] = visited[(nvisited - 1) - i];

	return req + size;
}

// This function checks if the path has a small U-turn, that is,
// a polygon further in the path is adjacent to the first polygon
// in the path. If that happens, a shortcut is taken.
// This can happen if the target (T) location is at tile boundary,
// and we're (S) approaching it parallel to the tile edge.
// The choice at the vertex can be arbitrary, 
//  +---+---+
//  |:::|:::|
//  +-S-+-T-+
//  |:::|   | <-- the step can end up in here, resulting U-turn path.
//  +---+---+
static int fixupShortcuts(dtPolyRef* path, int npath, dtNavMeshQuery* navQuery)
{
	if (npath < 3)
		return npath;

	// Get connected polygons
	static const int maxNeis = 16;
	dtPolyRef neis[maxNeis];
	int nneis = 0;

	const dtMeshTile* tile = 0;
	const dtPoly* poly = 0;
	if (dtStatusFailed(navQuery->getAttachedNavMesh()->getTileAndPolyByRef(path[0], &tile, &poly)))
		return npath;

	for (unsigned int k = poly->firstLink; k != DT_NULL_LINK; k = tile->links[k].next)
	{
		const dtLink* link = &tile->links[k];
		if (link->ref != 0)
		{
			if (nneis < maxNeis)
				neis[nneis++] = link->ref;
		}
	}

	// If any of the neighbour polygons is within the next few polygons
	// in the path, short cut to that polygon directly.
	static const int maxLookAhead = 6;
	int cut = 0;
	for (int i = dtMin(maxLookAhead, npath) - 1; i > 1 && cut == 0; i--) {
		for (int j = 0; j < nneis; j++)
		{
			if (path[i] == neis[j]) {
				cut = i;
				break;
			}
		}
	}
	if (cut > 1)
	{
		int offset = cut - 1;
		npath -= offset;
		for (int i = 1; i < npath; i++)
			path[i] = path[i + offset];
	}

	return npath;
}

static bool getSteerTarget(dtNavMeshQuery* navQuery, const float* startPos, const float* endPos,
	const float minTargetDist,
	const dtPolyRef* path, const int pathSize,
	float* steerPos, unsigned char& steerPosFlag, dtPolyRef& steerPosRef,
	float* outPoints = 0, int* outPointCount = 0)
{
	// Find steer target.
	static const int MAX_STEER_POINTS = 3;
	float steerPath[MAX_STEER_POINTS * 3];
	unsigned char steerPathFlags[MAX_STEER_POINTS];
	dtPolyRef steerPathPolys[MAX_STEER_POINTS];
	int nsteerPath = 0;
	navQuery->findStraightPath(startPos, endPos, path, pathSize,
		steerPath, steerPathFlags, steerPathPolys, &nsteerPath, MAX_STEER_POINTS);
	if (!nsteerPath)
		return false;

	if (outPoints && outPointCount)
	{
		*outPointCount = nsteerPath;
		for (int i = 0; i < nsteerPath; ++i)
			dtVcopy(&outPoints[i * 3], &steerPath[i * 3]);
	}


	// Find vertex far enough to steer to.
	int ns = 0;
	while (ns < nsteerPath)
	{
		// Stop at Off-Mesh link or when point is further than slop away.
		if ((steerPathFlags[ns] & DT_STRAIGHTPATH_OFFMESH_CONNECTION) ||
			!inRange(&steerPath[ns * 3], startPos, minTargetDist, 1000.0f))
			break;
		ns++;
	}
	// Failed to find good point to steer to.
	if (ns >= nsteerPath)
		return false;

	dtVcopy(steerPos, &steerPath[ns * 3]);
	steerPos[1] = startPos[1];
	steerPosFlag = steerPathFlags[ns];
	steerPosRef = steerPathPolys[ns];

	return true;
}


UnityGeom::UnityGeom()
{
	m_pNavMesh = NULL;
	m_pNavQuery = NULL;
	m_filter.setIncludeFlags(0x01); // SAMPLE_POLYFLAGS_WALK
	m_filter.setExcludeFlags(0);
	memset(m_bmin, 0, sizeof(m_bmin));
	memset(m_bmax, 0, sizeof(m_bmax));
}

UnityGeom::~UnityGeom()
{
	UnLoad();
}

bool UnityGeom::Load(const std::string& filepath)
{
	m_vertices.clear();
	m_indices.clear();
	m_triangles.clear();

	// 读取文件中的数据
	std::ifstream fin(filepath);
	std::string strVertices;
	getline(fin, strVertices);
	std::string strIndices;
	getline(fin, strIndices);

	// 把第一行数据拆分成一个一个的顶点
	size_t pos = strVertices.find(';');
	while (pos != std::string::npos)
	{
		std::string strTemp = strVertices.substr(0, pos);
		strVertices = strVertices.substr(pos + 1);
		pos = strVertices.find(';');

		size_t t = strTemp.find(',');
		std::string x = strTemp.substr(0, t);
		strTemp = strTemp.substr(t + 1);
		t = strTemp.find(',');
		std::string y = strTemp.substr(0, t);
		std::string z = strTemp.substr(t + 1);
		UnityVertex vertex;
		vertex.x = (float)atof(x.c_str());
		vertex.y = (float)atof(y.c_str());
		vertex.z = (float)atof(z.c_str());
		m_vertices.push_back(vertex);
	}
	size_t t = strVertices.find(',');
	std::string x = strVertices.substr(0, t);
	strVertices = strVertices.substr(t + 1);
	t = strVertices.find(',');
	std::string y = strVertices.substr(0, t);
	std::string z = strVertices.substr(t + 1);
	UnityVertex vertex;
	vertex.x = (float)atof(x.c_str());
	vertex.y = (float)atof(y.c_str());
	vertex.z = (float)atof(z.c_str());
	m_vertices.push_back(vertex);

	// 把第二行数据拆分成一个一个的索引
	pos = strIndices.find(';');
	while (pos != std::string::npos)
	{
		std::string strIndex = strIndices.substr(0, pos);
		strIndices = strIndices.substr(pos + 1);
		pos = strIndices.find(';');

		int index = atoi(strIndex.c_str());
		m_indices.push_back(index);
	}
	int index = atoi(strIndices.c_str());
	m_indices.push_back(index);

	int a = m_vertices.size();
	int b = m_indices.size();
	if (m_vertices.size() == 0 || m_indices.size() == 0)
	{
		return false;
	}

	// 根据顶点和索引数据，生成一个一个的三角型
	for (int i = 0; i < m_indices.size(); )
	{
		UnityTriangle tri;
		for (int j = 0; j < 3; ++j)
		{
			if (m_indices[i] < 0 || m_indices[i] >= m_vertices.size())
			{
				return false;
			}
			tri.vertices[j] = m_vertices[m_indices[i]];
			tri.indices[j] = m_indices[i];
			++i;
		}
		m_triangles.push_back(tri);
	}

	for (int i = 0; i < m_vertices.size(); ++i)
	{
		if (m_vertices[i].x < m_bmin[0]) m_bmin[0] = m_vertices[i].x;
		if (m_vertices[i].y < m_bmin[1]) m_bmin[1] = m_vertices[i].y;
		if (m_vertices[i].z < m_bmin[2]) m_bmin[2] = m_vertices[i].z;
		if (m_vertices[i].x > m_bmax[0]) m_bmax[0] = m_vertices[i].x;
		if (m_vertices[i].y > m_bmax[1]) m_bmax[1] = m_vertices[i].y;
		if (m_vertices[i].z > m_bmax[2]) m_bmax[2] = m_vertices[i].z;
	}

	dtNavMeshCreateParams params;
	memset(&params, 0, sizeof(params));
	params.nvp = 3;
	params.vertCount = (int)m_vertices.size();
	params.polyCount = (int)m_triangles.size();
	params.walkableHeight = 2;
	params.walkableRadius = 0.5f;
	params.walkableClimb = 0.5f;
	params.cs = 1;
	params.ch = 1;
	memcpy(params.bmin, m_bmin, sizeof(float) * 3);
	memcpy(params.bmax, m_bmax, sizeof(float) * 3);

	// 填充顶点
	unsigned short* pVerts = new unsigned short[3 * params.vertCount];
	for (int i = 0; i < params.vertCount; ++i)
	{
		unsigned short x = (unsigned short)round((m_vertices[i].x - m_bmin[0]) / params.cs);
		pVerts[i * 3] = x;
		unsigned short y = (unsigned short)round((m_vertices[i].y - m_bmin[1]) / params.ch);
		pVerts[i * 3 + 1] = y;
		unsigned short z = (unsigned short)round((m_vertices[i].z - m_bmin[2]) / params.cs);
		pVerts[i * 3 + 2] = z;
	}
	params.verts = pVerts;

	// 填充三角型
	unsigned short* pPolys = new unsigned short[params.polyCount * 6];
	for (int i = 0; i < params.polyCount; ++i)
	{
		int* a = m_triangles[i].indices;

		int v = i * 6;
		pPolys[v + 0] = (unsigned short)a[0];
		pPolys[v + 1] = (unsigned short)a[1];
		pPolys[v + 2] = (unsigned short)a[2];
		pPolys[v + 3] = 0xffff; // RC_MESH_NULL_IDX
		pPolys[v + 4] = 0xffff; // RC_MESH_NULL_IDX
		pPolys[v + 5] = 0xffff; // RC_MESH_NULL_IDX

		for (int k = 0; k < params.polyCount; ++k)
		{
			if (k == i) continue;
			int* b = m_triangles[k].indices;

			if ((a[0] == b[0] || a[0] == b[1] || a[0] == b[2]) &&
				(a[1] == b[0] || a[1] == b[1] || a[1] == b[2]))
			{
				pPolys[v + 3] = k;
			}
			if ((a[1] == b[0] || a[1] == b[1] || a[1] == b[2]) &&
				(a[2] == b[0] || a[2] == b[1] || a[2] == b[2]))
			{
				pPolys[v + 4] = k;
			}
			if ((a[2] == b[0] || a[2] == b[1] || a[2] == b[2]) &&
				(a[0] == b[0] || a[0] == b[1] || a[0] == b[2]))
			{
				pPolys[v + 5] = k;
			}
		}
	}
	params.polys = pPolys;

	// 填充三角型区域标志
	unsigned char* pAreas = new unsigned char[params.polyCount];
	unsigned short* pPolyFlags = new unsigned short[params.polyCount];
	for (int i = 0; i < params.polyCount; ++i)
	{
		pAreas[i] = 63; // RC_WALKABLE_AREA
		pPolyFlags[i] = 0x01; // SAMPLE_POLYFLAGS_WALK
	}
	params.polyAreas = pAreas;
	params.polyFlags = pPolyFlags;

	unsigned char* navData = NULL;
	int navDataSize = 0;
	if (!dtCreateNavMeshData(&params, &navData, &navDataSize))
	{
		delete pVerts;
		delete pPolys;
		delete pAreas;
		delete pPolyFlags;
		return false;
	}
	delete pVerts;
	delete pPolys;
	delete pAreas;
	delete pPolyFlags;

	m_pNavMesh = dtAllocNavMesh();
	if (m_pNavMesh == NULL)
	{
		dtFree(navData);
		return false;
	}

	dtStatus status;
	status = m_pNavMesh->init(navData, navDataSize, DT_TILE_FREE_DATA);
	if (dtStatusFailed(status))
	{
		dtFree(navData);
		return false;
	}

	m_pNavQuery = dtAllocNavMeshQuery();
	if (m_pNavQuery == NULL)
	{
		dtFree(navData);
		return false;
	}
	status = m_pNavQuery->init(m_pNavMesh, 2048);
	if (dtStatusFailed(status))
	{
		dtFree(navData);
		return false;
	}
	return true;
}

void UnityGeom::UnLoad()
{
	if (m_pNavQuery != NULL) delete m_pNavQuery;
	if (m_pNavMesh != NULL) delete m_pNavMesh;
}

bool UnityGeom::GetHeight(float x, float z, float& y)
{
	if (m_pNavQuery == NULL)
	{
		return false;
	}
	dtStatus status;
	float point[3] = { x, 0, z };
	float t[3] = { 0, 1000, 0 };
	float result[3] = { 0, 0, 0 };
	dtPolyRef ref;
	status = m_pNavQuery->findNearestPoly(point, t, &m_filter, &ref, result);
	if (dtStatusFailed(status))
	{
		return false;
	}
	if (result[0] != x || result[2] != z)
	{
		return false;
	}
	y = result[1];
	return true;
}

bool UnityGeom::FindPath(const UnityVertex& start, const UnityVertex& end, std::vector<UnityVertex>& path)
{
	if (m_pNavQuery == NULL)
	{
		return false;
	}

	dtStatus status;

	float s[3] = { start.x, start.y, start.z };
	float e[3] = { end.x, end.y, end.z };
	float t[3] = { 0, 1000, 0 };
	dtPolyRef startRef;
	status = m_pNavQuery->findNearestPoly(s, t, &m_filter, &startRef, 0);
	if (dtStatusFailed(status))
	{
		return false;
	}
	dtPolyRef endRef;
	status = m_pNavQuery->findNearestPoly(e, t, &m_filter, &endRef, 0);
	if (dtStatusFailed(status))
	{
		return false;
	}
	dtPolyRef polys[MAX_POLYS];
	int npolys;
	status = m_pNavQuery->findPath(startRef, endRef, s, e, &m_filter, polys, &npolys, MAX_POLYS);
	if (dtStatusFailed(status))
	{
		return false;
	}
	if (npolys <= 0)
	{
		return false;
	}

	static const int MAX_SMOOTH = 2048;
	static const float STEP_SIZE = 0.5f;
	static const float SLOP = 0.01f;

	int nsmoothPath = 0;
	float smoothPath[MAX_SMOOTH * 3];

	float iterPos[3], targetPos[3];
	m_pNavQuery->closestPointOnPoly(startRef, s, iterPos, 0);
	m_pNavQuery->closestPointOnPoly(polys[npolys - 1], e, targetPos, 0);

	memcpy(&smoothPath[nsmoothPath * 3], iterPos, sizeof(float) * 3);
	nsmoothPath++;
	UnityVertex vertex;
	vertex.x = iterPos[0];
	vertex.y = iterPos[1];
	vertex.z = iterPos[2];
	path.push_back(vertex);

	// Move towards target a small advancement at a time until target reached or
	// when ran out of memory to store the path.
	while (npolys && nsmoothPath < MAX_SMOOTH)
	{
		// Find location to steer towards.
		float steerPos[3];
		unsigned char steerPosFlag;
		dtPolyRef steerPosRef;

		if (!getSteerTarget(m_pNavQuery, iterPos, targetPos, SLOP,
			polys, npolys, steerPos, steerPosFlag, steerPosRef))
		{
			break;
		}

		bool endOfPath = (steerPosFlag & DT_STRAIGHTPATH_END) ? true : false;
		bool offMeshConnection = (steerPosFlag & DT_STRAIGHTPATH_OFFMESH_CONNECTION) ? true : false;

		// Find movement delta.
		float delta[3], len;
		dtVsub(delta, steerPos, iterPos);
		len = dtMathSqrtf(dtVdot(delta, delta));
		// If the steer target is end of path or off-mesh link, do not move past the location.
		if ((endOfPath || offMeshConnection) && len < STEP_SIZE)
			len = 1;
		else
			len = STEP_SIZE / len;
		float moveTgt[3];
		dtVmad(moveTgt, iterPos, delta, len);

		// Move
		float result[3];
		dtPolyRef visited[16];
		int nvisited = 0;
		m_pNavQuery->moveAlongSurface(polys[0], iterPos, moveTgt, &m_filter,
			result, visited, &nvisited, 16);

		npolys = fixupCorridor(polys, npolys, MAX_POLYS, visited, nvisited);
		npolys = fixupShortcuts(polys, npolys, m_pNavQuery);

		float h = 0;
		m_pNavQuery->getPolyHeight(polys[0], result, &h);
		result[1] = h;
		dtVcopy(iterPos, result);

		// Handle end of path and off-mesh links when close enough.
		if (endOfPath && inRange(iterPos, steerPos, SLOP, 1.0f))
		{
			// Reached end of path.
			dtVcopy(iterPos, targetPos);
			if (nsmoothPath < MAX_SMOOTH)
			{
				dtVcopy(&smoothPath[nsmoothPath * 3], iterPos);
				nsmoothPath++;
				UnityVertex vertex;
				vertex.x = iterPos[0];
				vertex.y = iterPos[1];
				vertex.z = iterPos[2];
				path.push_back(vertex);
			}
			break;
		}
		else if (offMeshConnection && inRange(iterPos, steerPos, SLOP, 1.0f))
		{
			// Reached off-mesh connection.
			float startPos[3], endPos[3];

			// Advance the path up to and over the off-mesh connection.
			dtPolyRef prevRef = 0, polyRef = polys[0];
			int npos = 0;
			while (npos < npolys && polyRef != steerPosRef)
			{
				prevRef = polyRef;
				polyRef = polys[npos];
				npos++;
			}
			for (int i = npos; i < npolys; ++i)
				polys[i - npos] = polys[i];
			npolys -= npos;

			// Handle the connection.
			dtStatus status = m_pNavMesh->getOffMeshConnectionPolyEndPoints(prevRef, polyRef, startPos, endPos);
			if (dtStatusSucceed(status))
			{
				if (nsmoothPath < MAX_SMOOTH)
				{
					dtVcopy(&smoothPath[nsmoothPath * 3], startPos);
					nsmoothPath++;
					UnityVertex vertex;
					vertex.x = iterPos[0];
					vertex.y = iterPos[1];
					vertex.z = iterPos[2];
					path.push_back(vertex);
					// Hack to make the dotted path not visible during off-mesh connection.
					if (nsmoothPath & 1)
					{
						dtVcopy(&smoothPath[nsmoothPath * 3], startPos);
						nsmoothPath++;
						UnityVertex vertex;
						vertex.x = iterPos[0];
						vertex.y = iterPos[1];
						vertex.z = iterPos[2];
						path.push_back(vertex);
					}
				}
				// Move position at the other side of the off-mesh link.
				dtVcopy(iterPos, endPos);
				float eh = 0.0f;
				m_pNavQuery->getPolyHeight(polys[0], iterPos, &eh);
				iterPos[1] = eh;
			}
		}

		// Store results.
		if (nsmoothPath < MAX_SMOOTH)
		{
			dtVcopy(&smoothPath[nsmoothPath * 3], iterPos);
			nsmoothPath++;
			UnityVertex vertex;
			vertex.x = iterPos[0];
			vertex.y = iterPos[1];
			vertex.z = iterPos[2];
			path.push_back(vertex);
		}
	}
	return true;
}

