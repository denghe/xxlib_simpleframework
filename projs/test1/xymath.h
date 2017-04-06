#pragma once
#include <cassert>
#include <cmath>
#include <array>
#include <limits>
#include "xx_structs.h"
#include "xx_list.h"

using XY = xx::XY<float>;
struct XyMath
{
	static constexpr double pi = 3.14159265358979323846;
	static constexpr double digiNum = 100;							// 当前单位是 float 米, * 100 转为厘米
	static constexpr int xyTableDiameter = 2048						// 怪普遍的警戒距离(半径) 为 10 米以内
		, xyTableRadius = xyTableDiameter / 2
		, xyTableRadiusReduce = xyTableRadius - 1;
	static constexpr int angleCount = 256							// 当前是按 1字节 的精度来处理角度的
		, halfAngleCount = angleCount / 2;

	std::array<XY, angleCount> xyIncs;
	std::array<uint8_t, xyTableDiameter * xyTableDiameter> angles;

	XyMath()
	{
		FillXyIncs();
		FillAngle();
	}

	// 填充角度对应的标准增量值
	inline void FillXyIncs()
	{
		XY xyInc;
		for (int i = 0; i < angleCount; i++)
		{
			auto a = pi * i / halfAngleCount;
			xyInc.x = (float)std::cos(a);
			xyInc.y = (float)std::sin(a);
			xyIncs[i] = xyInc;
		}
	}

	// 填充一个片区坐标与朝向角度的对应
	inline void FillAngle()
	{
		for (int i = 0; i < xyTableDiameter; i++)
		{
			for (int j = 0; j < xyTableDiameter; j++)
			{
				auto idx = i * xyTableDiameter + j;

				auto x = j - xyTableRadius;
				auto y = i - xyTableRadius;

				auto v = (int)(std::atan2(i - xyTableRadius, j - xyTableRadius) / pi * halfAngleCount);
				if (v >= 0) angles[idx] = (uint8_t)v;
				else angles[idx] = (uint8_t)(angleCount + v);
			}
		}
	}

	inline uint8_t GetAngle(int x, int y)
	{
		// 如果 x 或 y 大于 hlen 则需要将 xy 等比缩小
		auto x_ = std::abs(x);
		auto y_ = std::abs(y);
		if (x_ >= xyTableRadius || y_ >= xyTableRadius)
		{
			if (x_ >= y_)
			{
				y = (int)(y * (double)xyTableRadiusReduce / x_);
				x = x == x_ ? xyTableRadiusReduce : -xyTableRadiusReduce;
			}
			else
			{
				x = (int)(x * (double)xyTableRadiusReduce / y_);
				y = y == y_ ? xyTableRadiusReduce : -xyTableRadiusReduce;
			}
		}
		return angles[(y + xyTableRadius) * xyTableDiameter + (x + xyTableRadius)];
	}

	// 根据矢量算角度朝向( 通常传入 target.xy - this.xy )
	// 将 "米" 放大到 "厘米" 来计算以确保精度( 通常用于矢量叠加判断受力方向的场合 )
	inline uint8_t GetAngle(XY xy)
	{
		return GetAngle((int)(xy.x * digiNum), (int)(xy.y * digiNum));
	}


	// 根据矢量获取标准增量
	inline XY GetXyInc(XY xy)
	{
		return xyIncs[GetAngle(xy)];
	}
	inline XY GetXyInc(int x, int y)
	{
		return xyIncs[GetAngle(x, y)];
	}
	inline XY GetXyInc(uint8_t a)
	{
		return xyIncs[a];
	}


	// 算距离相关
	inline static float GetDistance(int x, int y)
	{
		return sqrtf((float)(x * x + y * y));
	}
	inline static float GetDistance(XY xy)
	{
		return GetDistance((int)xy.x, (int)xy.y);
	}
	inline static float GetDistancePow2(XY a)
	{
		return a.x * a.x + a.y * a.y;
	}


	// 就近往一个角度偏转. 返回角度结果
	// a 每次产生不超过 inc 角度的变化, 就近向 b 角度接近, 最后完全等于
	// a, b 的值范围为 0 - 255, 右侧逆时针方向转动. inc 必须为正整数
	inline static uint8_t ChangeAngle(int a, int b, int inc)
	{
		if (a == b) return (uint8_t)a;
		else if (a > b)
		{
			if (a - b > halfAngleCount)
			{
				a += inc;
				if (a >= b + angleCount) return (uint8_t)b;
				return (uint8_t)a;
			}
			else
			{
				a -= inc;
				if (a <= b) return (uint8_t)b;
				return (uint8_t)a;
			}
		}
		else
		{
			if (b - a > halfAngleCount)
			{
				a -= inc;
				if (a <= b - angleCount) return (uint8_t)b;
				return (uint8_t)a;
			}
			else
			{
				a += inc;
				if (a >= b) return (uint8_t)b;
				return (uint8_t)a;
			}
		}
	}

	// 计算 0 -> pos 旋转 a 之后的新的 pos
	inline XY RotateXY(XY pos, uint8_t a)
	{
		auto sincos = GetXyInc(a);
		auto sinA = sincos.y;
		auto cosA = sincos.x;
		return XY{ pos.x * cosA - pos.y * sinA, pos.x * sinA + pos.y * cosA };
	}

	// 角度转为 360 度左手系 保留 2 位精度的整数
	inline static int ConvertToXZAngleForSend(uint8_t a)
	{
		return (int)(float((uint8_t)(-(int8_t)a + 64)) / 256.0f * 36000.0f);
	}

	// 从 360 度配置读入角度值
	inline static uint8_t ConvertFromXZ360Angle(int a)
	{
		return (uint8_t)(-(int8_t)(uint8_t)(a / 360.0f * 256.0f) + 64);
	}

	// 点乘
	inline static float Dot(XY const& p1, XY const& p2)
	{
		XY p = p1 * p2;
		return p.x + p.y;
	}

	// 倒数开方
	inline static float Inversesqrt(float const& x)
	{
		return 1.0f / sqrtf(x);
	}

	// 归一化( 理论上讲可以用 GetXyInc 来替代, 只不过精度较低, 恐出问题 )
	inline static XY Normalize(XY const& p)
	{
		return p * Inversesqrt(Dot(p, p));
	}

	// 填充矩形( 多边型 )的顶点
	inline void FillRectPoints(XY centPos, uint8_t directionAngle, float w, float h, xx::List<XY>& outPoints)
	{
		outPoints.Resize(4);
		XY rectXyInc = GetXyInc(directionAngle);
		XY rectYxInc{ rectXyInc.y, -rectXyInc.x };
		outPoints[0] = centPos - (rectYxInc * w + rectXyInc * h) * 0.5f;
		outPoints[1] = outPoints[0] + rectYxInc * w;
		outPoints[2] = outPoints[1] + rectXyInc * h;
		outPoints[3] = outPoints[0] + rectXyInc * h;
	}

	// 填充扇形( 多边型 )的顶点
	inline void FillFanPoints(XY pos, uint8_t directionAngle, uint8_t fanAngle, float radius, int segmentCount, xx::List<XY>& outPoints)
	{
		outPoints.Resize(segmentCount + 2);
		outPoints[0] = pos;
		auto beginAngle = (uint8_t)(directionAngle - fanAngle / 2);
		outPoints[1] = pos + (GetXyInc(beginAngle) * radius);
		for (auto i = 1; i <= segmentCount; ++i)
		{
			auto a = (uint8_t)(beginAngle + (float)fanAngle * i / segmentCount);
			outPoints[i + 1] = pos + (GetXyInc(a) * radius);
		}
	}

	// 根据多边型顶点填充正交投影
	inline static void FillProjectionAxis(xx::List<XY>& points, xx::List<XY>& projectionAxis)
	{
		auto count = (int)points.dataLen;
		points.Reserve(points.dataLen + 1);
		points[count] = points[0];			// 营造一个闭合形状 避免循环内 if
		projectionAxis.Resize(count + 1);
		for (auto i = 0; i < count - 1; i++)
		{
			auto edge = points[i + 1] - points[i];
			projectionAxis[i] = Normalize({ edge.y, -edge.x });
		}
	}

	// 判断圆与多边型是否相交
	inline static bool CirclePolygonIntersect(XY circleCenter, float radius, xx::List<XY> const& points, xx::List<XY>& projectionAxis)
	{
		auto count = (int)points.dataLen;

		auto closedXY = circleCenter - points[0];
		auto closedDis = GetDistancePow2(closedXY);
		for (auto i = 0; i < count; i++)
		{
			// 找到多边形和圆形最近的点
			auto deltaXy = circleCenter - points[i];
			auto dis = GetDistancePow2(deltaXy);
			if (dis < closedDis)
			{
				closedXY = deltaXy;
				closedDis = dis;
			}
		}
		projectionAxis[count] = Normalize(closedXY);

		// 在每个投影轴上投影多边形和圆形,看是否有相交
		for (auto j = 0; j <= count; j++)
		{
			auto& axis = projectionAxis[j];
			auto circleMaxProjection = Dot(circleCenter, axis) + radius;
			auto circleMinProjection = circleMaxProjection - 2 * radius;

			auto rectMaxProjection = std::numeric_limits<float>::min();
			auto rectMinProjection = std::numeric_limits<float>::max();

			for (auto i = 0; i < count; i++)
			{
				auto projection = Dot(points[i], axis);
				rectMaxProjection = MAX(rectMaxProjection, projection);
				rectMinProjection = MIN(rectMinProjection, projection);
			}
			if (MIN(circleMaxProjection, rectMaxProjection) <= MAX(circleMinProjection, rectMinProjection))
			{
				return false;
			}
		}

		return true;
	}
};

extern XyMath xyMath;			// 需要在某个 cpp 中实现
