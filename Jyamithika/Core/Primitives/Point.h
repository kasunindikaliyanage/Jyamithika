#pragma once

#include "Vector.h"

namespace jmk {

	typedef Vector2f Point2d;
	typedef Vector3f Point3d;

	template<typename coordinate_type, size_t dimensions>
	bool sort2DLRTB(const Point3d& a, const Point3d& b)
	{
		if ((a[X] < b[X])
			|| (a[X] == b[X]) && (a[Y] < b[Y]))
		{
			return true;
		}
		return false;
	}

	template<typename coordinate_type, size_t dimensions>
	bool sort2DTBLR(const Point3d& a, const Point3d& b)
	{
		if ((a[Y] > b[Y])
			|| (a[Y] == b[Y]) && (a[X] < b[X]))
		{
			return true;
		}
		return false;
	}

	template<typename coordinate_type, size_t dimensions>
	float distance(Point3d& p1, Point3d& p2)
	{
		float dx = p1[X] - p2[X];
		float dy = p1[Y] - p2[Y];
		float dz = p1[Z] - p2[Z];

		float distance = sqrt(pow(dx, 2) + pow(dy, 2) + pow(dz, 2));
		return distance;
	}
}