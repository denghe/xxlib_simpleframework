#include "UnityGeom.h"
#include <cstdint>
#include <iostream>

// 角度转为 360 度左手系 保留 2 位精度的整数
int ConvertToXZAngleForSend(uint8_t a)
{
	return (int)(float((uint8_t)(-(int8_t)a + 64)) / 256.0f * 36000.0f);
}

// 从 360 度配置读入角度值
uint8_t ConvertFromXZ360Angle(int a)
{
	return (uint8_t)(-(int8_t)(uint8_t)(a / 360.0f * 256.0f) + 64);
}

int main()
{
	std::cout << ConvertToXZAngleForSend(128) << std::endl;
	std::cout << (int)ConvertFromXZ360Angle(180) << std::endl;


	return 0;
	UnityGeom ug;
	ug.Load("1000.bytes");
	float y;
	auto b = ug.GetHeight(10.8f, 24.8f, y);
	return 0;
}
