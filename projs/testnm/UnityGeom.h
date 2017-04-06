
#pragma once

#include <string>
#include <vector>
#include "DetourCommon.h"
#include "DetourNavMesh.h"
#include "DetourNavMeshBuilder.h"
#include "DetourNavMeshQuery.h"

enum SamplePolyFlags
{
	SAMPLE_POLYFLAGS_WALK = 0x01,		// Ability to walk (ground, grass, road)
	SAMPLE_POLYFLAGS_SWIM = 0x02,		// Ability to swim (water).
	SAMPLE_POLYFLAGS_DOOR = 0x04,		// Ability to move through doors.
	SAMPLE_POLYFLAGS_JUMP = 0x08,		// Ability to jump.
	SAMPLE_POLYFLAGS_DISABLED = 0x10,		// Disabled polygon
	SAMPLE_POLYFLAGS_ALL = 0xffff	// All abilities.
};

enum SamplePolyAreas
{
	SAMPLE_POLYAREA_GROUND,
	SAMPLE_POLYAREA_WATER,
	SAMPLE_POLYAREA_ROAD,
	SAMPLE_POLYAREA_DOOR,
	SAMPLE_POLYAREA_GRASS,
	SAMPLE_POLYAREA_JUMP,
};

// 顶点结构
struct UnityVertex
{
	float x; // x坐标
	float y; // y坐标
	float z; // z坐标
};

// 三角型结构
struct UnityTriangle
{
	UnityVertex vertices[3]; // 顶点
	int indices[3]; // 索引
};

class UnityGeom
{
public:
	UnityGeom();
	~UnityGeom();

	bool Load(const std::string& filepath); // 加载地图navmesh数据
	void UnLoad(); // 卸载navmesh数据
	bool FindPath(const UnityVertex& start, const UnityVertex& end, std::vector<UnityVertex>& path); // 寻路
	bool GetHeight(float x, float z, float& y); // 获取到xz平面上一点在navmesh上的高度，如果该点不在navmesh上，那么返回false
	void GetBound(UnityVertex& min, UnityVertex& max); // 获得包围盒最小点和最大点数据

	const std::vector<UnityTriangle>& GetTriangles() { return m_triangles; } // 获取到三角型列表
	const std::vector<UnityVertex>& GetVertices() { return m_vertices; } // 获取到顶点列表
	const std::vector<int>& GetIndices() { return m_indices; } // 获取到索引列表

private:
	std::vector<UnityVertex> m_vertices;
	std::vector<int> m_indices;
	std::vector<UnityTriangle> m_triangles;
	float m_bmin[3];
	float m_bmax[3];
	dtNavMesh* m_pNavMesh;
	dtNavMeshQuery* m_pNavQuery;
	dtQueryFilter m_filter;
	static const int MAX_POLYS = 256;
};

/// Returns the minimum of two values.
///  @param[in]		a	Value A
///  @param[in]		b	Value B
///  @return The minimum of the two values.
template<class T> inline T rcMin(T a, T b) { return a < b ? a : b; }

/// Returns the maximum of two values.
///  @param[in]		a	Value A
///  @param[in]		b	Value B
///  @return The maximum of the two values.
template<class T> inline T rcMax(T a, T b) { return a > b ? a : b; }