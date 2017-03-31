//
// Copyright (c) 2009-2010 Mikko Mononen memon@inside.org
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//

#ifndef DETOURNAVMESHBUILDER_H
#define DETOURNAVMESHBUILDER_H

#include "DetourAlloc.h"

/// Represents the source data used to build an navigation mesh tile.
// 表示用于构建导航网格贴图的源数据。
/// @ingroup detour
struct dtNavMeshCreateParams
{

	/// @name Polygon Mesh Attributes
	/// Used to create the base navigation graph. 用于创建基本导航图。
	/// See #rcPolyMesh for details related to these attributes. 有关这些属性的详细信息，请参阅#rcPolyMesh。
	/// @{

	const unsigned short* verts;			///< The polygon mesh vertices. [(x, y, z) * #vertCount] [Unit: vx] 多边形网格顶点。
	int vertCount;							///< The number vertices in the polygon mesh. [Limit: >= 3] 多边形网格中的顶点数。
	const unsigned short* polys;			///< The polygon data. [Size: #polyCount * 2 * #nvp] 多边形数据。
	const unsigned short* polyFlags;		///< The user defined flags assigned to each polygon. [Size: #polyCount] 用户定义分配给每个多边形的标志。
	const unsigned char* polyAreas;			///< The user defined area ids assigned to each polygon. [Size: #polyCount] 分配给每个多边形的用户定义区域ID。
	int polyCount;							///< Number of polygons in the mesh. [Limit: >= 1] 网格中的多边形数量。
	int nvp;								///< Number maximum number of vertices per polygon. [Limit: >= 3] 每个多边形的顶点数量的最大值。

	/// @}
	/// @name Height Detail Attributes (Optional) 高度详细信息属性（可选的）
	/// See #rcPolyMeshDetail for details related to these attributes. 有关这些属性的详细信息，请参阅#rcPolyMeshDetail。
	/// @{

	const unsigned int* detailMeshes;		///< The height detail sub-mesh data. [Size: 4 * #polyCount] 高度详细子网格数据。
	const float* detailVerts;				///< The detail mesh vertices. [Size: 3 * #detailVertsCount] [Unit: wu] 细节网格顶点。
	int detailVertsCount;					///< The number of vertices in the detail mesh. 细节网格中的顶点数。
	const unsigned char* detailTris;		///< The detail mesh triangles. [Size: 4 * #detailTriCount] 细节网格三角形。
	int detailTriCount;						///< The number of triangles in the detail mesh. 细节网格中的三角形数量。

	/// @}
	/// @name Off-Mesh Connections Attributes (Optional) 离网连接属性（可选）
	/// Used to define a custom point-to-point edge within the navigation graph, an 
	/// off-mesh connection is a user defined traversable connection made up to two vertices, 
	/// at least one of which resides within a navigation mesh polygon.
	/// 用于在导航图内定义自定义点到点边缘，离网连接是由两个顶点组成的用户定义的可穿越连接，
	/// 其中至少一个顶点位于导航网格多边形内。
	/// @{

	/// Off-mesh connection vertices. [(ax, ay, az, bx, by, bz) * #offMeshConCount] [Unit: wu] 离网连接顶点。
	const float* offMeshConVerts;
	/// Off-mesh connection radii. [Size: #offMeshConCount] [Unit: wu] 离网连接半径。
	const float* offMeshConRad;
	/// User defined flags assigned to the off-mesh connections. [Size: #offMeshConCount] 分配给离网连接的用户定义标志。
	const unsigned short* offMeshConFlags;
	/// User defined area ids assigned to the off-mesh connections. [Size: #offMeshConCount] 分配给离网连接的用户定义的区域ID。
	const unsigned char* offMeshConAreas;
	/// The permitted travel direction of the off-mesh connections. [Size: #offMeshConCount] 离网连接的允许行进方向。
	///
	/// 0 = Travel only from endpoint A to endpoint B. 0 = 仅从端点A行进到端点B.<br/>
	/// #DT_OFFMESH_CON_BIDIR = Bidirectional travel. #DT_OFFMESH_CON_BIDIR = 双向行驶。
	const unsigned char* offMeshConDir;	
	/// The user defined ids of the off-mesh connection. [Size: #offMeshConCount] 用户定义的离网连接的ids。
	const unsigned int* offMeshConUserID;
	/// The number of off-mesh connections. [Limit: >= 0] 离网连接的数量。
	int offMeshConCount;

	/// @}
	/// @name Tile Attributes 图块属性
	/// @note The tile grid/layer data can be left at zero if the destination is a single tile mesh. 如果目的地是单个瓦片网格，则瓦片网格/层数据可以保留为零。
	/// @{

	unsigned int userId;	///< The user defined id of the tile. 用户定义的图块的ID。
	int tileX;				///< The tile's x-grid location within the multi-tile destination mesh. (Along the x-axis.) 瓦片在多瓦目的地网格内的x位置。
	int tileY;				///< The tile's y-grid location within the multi-tile desitation mesh. (Along the z-axis.) 瓦片在多瓦目的地网格内的y位置。
	int tileLayer;			///< The tile's layer within the layered destination mesh. [Limit: >= 0] (Along the y-axis.) 分层目标网格中的图层的图层。
	float bmin[3];			///< The minimum bounds of the tile. [(x, y, z)] [Unit: wu] 图块的最小边界。
	float bmax[3];			///< The maximum bounds of the tile. [(x, y, z)] [Unit: wu] 图块的最大边界。

	/// @}
	/// @name General Configuration Attributes 常规配置属性
	/// @{

	float walkableHeight;	///< The agent height. [Unit: wu] 代理高度。
	float walkableRadius;	///< The agent radius. [Unit: wu] 代理半径。
	float walkableClimb;	///< The agent maximum traversable ledge. (Up/Down) [Unit: wu] 代理最大可爬上多高的台阶。
	float cs;				///< The xz-plane cell size of the polygon mesh. [Limit: > 0] [Unit: wu] 多边形网格的xz平面单元格大小。
	float ch;				///< The y-axis cell height of the polygon mesh. [Limit: > 0] [Unit: wu] 多边形网格的y轴单元格高度。

	/// True if a bounding volume tree should be built for the tile. 如果应为图块构建包围体树，则为true。
	/// @note The BVTree is not normally needed for layered navigation meshes. 注意：分层导航网格通常不需要BVTree。
	bool buildBvTree;

	/// @}
};

bool myLoadUnityNavMeshData();

/// Builds navigation mesh tile data from the provided tile creation data.
/// @ingroup detour
///  @param[in]		params		Tile creation data.
///  @param[out]	outData		The resulting tile data.
///  @param[out]	outDataSize	The size of the tile data array.
/// @return True if the tile data was successfully created.
bool dtCreateNavMeshData(dtNavMeshCreateParams* params, unsigned char** outData, int* outDataSize);

/// Swaps the endianess of the tile data's header (#dtMeshHeader).
///  @param[in,out]	data		The tile data array.
///  @param[in]		dataSize	The size of the data array.
bool dtNavMeshHeaderSwapEndian(unsigned char* data, const int dataSize);

/// Swaps endianess of the tile data.
///  @param[in,out]	data		The tile data array.
///  @param[in]		dataSize	The size of the data array.
bool dtNavMeshDataSwapEndian(unsigned char* data, const int dataSize);

#endif // DETOURNAVMESHBUILDER_H

// This section contains detailed documentation for members that don't have
// a source file. It reduces clutter in the main section of the header.

/**

@struct dtNavMeshCreateParams
@par

This structure is used to marshal data between the Recast mesh generation pipeline and Detour navigation components.
此结构用于在Recast网格生成管道和Detour导航组件之间编组数据。
See the rcPolyMesh and rcPolyMeshDetail documentation for detailed information related to mesh structure.
有关网格结构的详细信息，请参阅rcPolyMesh和rcPolyMeshDetail文档。
Units are usually in voxels (vx) or world units (wu). The units for voxels, grid size, and cell size 
are all based on the values of #cs and #ch.
单位通常是体素（vx）或世界单位（wu）。 体素，网格大小和像元大小的单位
都基于#cs和#ch的值。
The standard navigation mesh build process is to create tile data using dtCreateNavMeshData, then add the tile 
to a navigation mesh using either the dtNavMesh single tile <tt>init()</tt> function or the dtNavMesh::addTile()
function.
标准导航网格构建过程是使用dtCreateNavMeshData创建磁贴数据，然后添加磁贴
导航网格使用dtNavMesh单个图块<tt> init（）</ tt>函数或dtNavMesh :: addTile（）
功能。
@see dtCreateNavMeshData

*/

