
#include "UnityGeom.h"
#include "DetourNavMeshQuery.h"
#include <iostream>
int main()
{
	UnityGeom geom;
	if (!geom.Load("1000.bytes"))
	{
		return 0;
	}

	UnityVertex start;
	start.x = 11.45f;
	start.y = 0;
	start.z = 9.42;
	UnityVertex end;
	end.x = 39.93f;
	end.y = 0;
	end.z = 25.92f;

	float y1;
	//bool r1 = geom.GetHeight(21.28f, 10.5f, y1);

	float y2;
	bool r2 = geom.GetHeight(10.8f, 24.8f, y2);
	//vector<UnityVertex> path;
	//geom.FindPath(start, end, path);
	std::cout << r2 << std::endl;
	return 0;
}