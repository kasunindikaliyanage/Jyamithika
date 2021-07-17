#pragma once

#include "Primitives\Point.h"

namespace jmk {

	// Return true if point [d] is inside the boundary of triangle defined by the points a, b, c
	bool isInside(Point3d& a, Point3d& b, Point3d& c, Point3d& d);

}