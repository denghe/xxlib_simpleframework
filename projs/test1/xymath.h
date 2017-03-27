#pragma once
#include <cassert>
#include <cmath>
#include <array>
#include "xx_structs.h"

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

	void FillXyIncs()
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
	void FillAngle()
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


	uint8_t GetAngle(int x, int y)
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

	// 将 "米" 放大到 "厘米" 来计算以确保精度
	uint8_t GetAngle(XY xy)
	{
		return GetAngle((int)(xy.x * digiNum), (int)(xy.y * digiNum));
	}


	XY GetXyInc(XY xy)
	{
		return xyIncs[GetAngle(xy)];
	}
	XY GetXyInc(int x, int y)
	{
		return xyIncs[GetAngle(x, y)];
	}
	XY GetXyInc(uint8_t a)
	{
		return xyIncs[a];
	}


	float GetDistance(int x, int y)
	{
		return std::sqrtf((float)(x * x + y * y));
	}
	float GetDistance(XY xy)
	{
		return GetDistance((int)xy.x, (int)xy.y);
	}
	float GetDistancePow2(XY a)
	{
		return a.x * a.x + a.y * a.y;
	}


	/*********************************************************************************************/
	// others
	/*********************************************************************************************/
	// a 每次产生不超过 inc 角度的变化, 就近向 b 角度接近, 最后完全等于
	// a, b 的值范围为 0 - 255, 右侧逆时针方向转动. inc 必须为正整数
	static uint8_t ChangeAngle(int a, int b, int inc)
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
	XY RotateXY(XY pos, uint8_t a)
	{
		auto sincos = GetXyInc(a);
		auto sinA = sincos.y;
		auto cosA = sincos.x;
		return XY{ pos.x * cosA - pos.y * sinA, pos.x * sinA + pos.y * cosA };
	}

};

extern XyMath xyMath;			// 需要在某个 cpp 中实现
