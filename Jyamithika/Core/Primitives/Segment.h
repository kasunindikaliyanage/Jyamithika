#pragma once
#include "Point.h"

namespace jmk {

	struct Segment {
		Point3d& p1;
		Point3d& p2;
	};

	struct Segment2d {
		Point2d p1  = DEFAULT_POINT_2D;
		Point2d p2  = DEFAULT_POINT_2D;
		Segment2d() {}

		//Segment2d(Point2d _p1, Point2d _p2) : p1(_p1), p2(_p2) {}
		Segment2d(Point2d& _p1, Point2d& _p2): p1(_p1),p2(_p2) {}

		double get_x(double y) const
		{
			double x1 = p1[X];
			double y1 = p1[Y];
			double x2 = p2[X];
			double y2 = p2[Y];

			double y2_y1 = y2 - y1;

			return y * (x2 - x1) / y2_y1 + (y2 * x1 - y1 * x2) / y2_y1;
		}
	};
}