#pragma once

#include "Vector.h"

namespace jmk {

	#define X 0
	#define Y 1
	#define Z 2

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
}