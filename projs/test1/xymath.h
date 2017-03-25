#pragma once
#include <cassert>
#include <cmath>
#include <array>

struct Range
{
	int from, to;
	inline bool Test(int n)
	{
		assert(from <= to);
		return n >= from && n <= to;
	}
};

struct XY
{
	double x, y;
};

struct XyMath
{
	static constexpr double pi = 3.14159265358979323846;
	static constexpr double digiNum = 100;							// 当前单位是 float 米, * 100 转为厘米
	static constexpr int angleCount = 256, halfAngleCount = angleCount / 2;
	static constexpr int xyTableDiameter = 1024, xyTableRadius = xyTableDiameter / 2, xyTableRadiusReduce = xyTableRadius - 1;

	static double Round(double x)
	{
		return std::ceil(x * digiNum) / digiNum;
	}

	std::array<XY, angleCount> xyIncs;
	std::array<uint8_t, xyTableDiameter * xyTableDiameter> angles;
	std::array<double, xyTableDiameter * xyTableDiameter> distances;

	XyMath()
	{
		FillXyIncs();
		FillAngleDistances();
	}

	void FillXyIncs()
	{
		XY xyInc;
		for (int i = 0; i < angleCount; i++)
		{
			auto a = pi * i / halfAngleCount;
			xyInc.x = Round(std::cos(a));
			xyInc.y = Round(std::sin(a));
			xyIncs[i] = xyInc;
		}
	}
	void FillAngleDistances()
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

				distances[idx] = Round(std::sqrt((double)(x * x + y * y)));
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

	// 考虑到多体碰撞需要用 xyInc 叠加来推算最后的方向, 故需要 double 版
	// 通常需要将值放大以查表
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


	double GetDistance(int x, int y)
	{
		return std::sqrt(x * x + y * y);// 恐有跨设备计算误差问题
	}
	double GetDistance(XY xy)
	{
		return GetDistance((int)xy.x, (int)xy.y);
	}
	double GetDistancePow2(XY a)
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
