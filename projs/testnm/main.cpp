#include "UnityGeom.h"
#include <cstdint>
#include <iostream>
#include "xx_mempool.h"
#include "../test1/xymath.h"

XyMath xyMath;

int main()
{
	{
		UnityGeom ug;
		ug.Load("1006.bytes");
		std::cout << "1006.bytes loaded." << std::endl;

		UnityVertex uvmin, uvmax;
		ug.GetBound(uvmin, uvmax);
		std::cout << "uvmin = " << uvmin.x << ", " << uvmin.y << ", " << uvmin.z << "";
		std::cout << "uvmax = " << uvmax.x << ", " << uvmax.y << ", " << uvmax.z << std::endl;

		std::vector<UnityVertex> path;
		//b = ug.FindPath(uv1, uv2, path);
		xx::Stopwatch sw;
		int count = 0;
		//float y;
		//b = ug.GetHeight(99, 48, y);
		//b = ug.GetHeight(239, 41, y);
		for (int i = 0; i < 1; ++i)
		{
			//auto b = ug.GetHeight(2.115, -1.399, y);
			path.clear();
			UnityVertex uv1{ 99, 0, 48 };
			auto b = ug.GetHeight(uv1.x, uv1.z, uv1.y);
			if (b) ++count;
			//UnityVertex uv2{ 237, 0, 40 };
			UnityVertex uv2{ 121, 0, 73 };
			b = ug.GetHeight(uv2.x, uv2.z, uv2.y);
			if (b) ++count;
			b = ug.FindPath(uv1, uv2, path);
			if (b) ++count;
		}
		std::cout << count << " " << path.size() << std::endl;
		std::cout << "ms = " << sw() << std::endl;

		// todo: 从 path 按距离枚举一个点, 直到枚举到头




		int offset = 1;													// 跳过起点( 这是需要保存在上下文中的状态值 )

		XY pos = { path[0].x, path[0].z };								// 模拟获取怪的当前位置( 查询返回的路径的首个点应该就是路径查询开始时的起点 )
		uint8_t angle;													// 模拟怪的当前角度

		float moveDistance = 0.5f;										// 本次移动距离( 模拟从配置读取, 每秒米 / 每秒帧 = 每帧移动长度 即帧移动距离 )
		while (moveDistance > 0  && offset < path.size())				// 原理是不断的向下一个移动, 直到满足距离或没有点为止
		{
			XY tar = { path[offset].x, path[offset].z };				// 取移动目标点
			auto dir = tar - pos;										// 算方向向量
			angle = xyMath.GetAngleUnsafe(dir);							// 查表得到 向量 角度
			auto disPow2 = xyMath.GetDistancePow2(dir);					// 算当前坐标与目标点的距离的平方
			if (moveDistance * moveDistance >= disPow2)					// 如果目标点不足以满足移动距离, 则将当前坐标移至该点, 并减去已经移动过的距离长, 跳过该点
			{
				pos = tar;
				moveDistance -= sqrtf(disPow2);
				++offset;
			}
			else														// 如果目标点满足移动距离, 则向 目标点 移动 当前点 距离长度
			{
				//pos = pos + xyMath.Normalize(dir) * moveDistance;		// 矢量归一化后 * 距离作为偏移量
				pos = pos + xyMath.GetXyInc(angle) * moveDistance;		// 查表法获取偏移量( 比开方更快但可能不是非常精确 )
				moveDistance = 0.0f;
			}
		}

		if (moveDistance != 0.0f)										// 探测循环结束原因: 点扫完了, 但还剩一段距离, 那就向着目标继续移动这段距离
		{
			//pos = pos + xyMath.GetXyInc(tar->pos - pos) * moveDistance;
		}

		// todo: 将 pos 和 angle 应用到怪


	}
	return 0;
}
