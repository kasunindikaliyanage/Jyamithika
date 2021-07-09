#pragma once

#include "Primitives\Point.h"

namespace jmk 
{
	// Returns counter clockwise angle (0 - 360) measure from referece point to the give point 
	float polarAngle( const Point3d& _other, const Point3d& _ref) ;

	// Return the area of the triangle defined by given 3 points
	double areaTriangle(Point3d& a, Point3d& b, Point3d& c);

	// Return the angle between two vectors
	float angle(Vector3f& _v1, Vector3f& _v2);
}


