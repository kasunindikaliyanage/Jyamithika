#pragma once
#include "Vector.h"
#include "Point.h"

namespace jmk {
	class Line {
		Vector3f dir;
		Point3d point_in_line;

	public:
		Line(Point3d& p1, Point3d& p2)
		{
			point_in_line = p1;
			dir = p2 - p1;
		}

		Vector3f direction();

		Point3d point();
	};
}