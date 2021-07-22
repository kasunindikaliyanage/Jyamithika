#pragma once
#include "Point.h"

namespace jmk {

	class Segment {
		Point3d& p1;
		Point3d& p2;
	};

	class Segment2d {
		Point2d* p1 = nullptr;
		Point2d* p2 = nullptr;
	public:	
		Segment2d() {}
	};
}