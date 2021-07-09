#pragma once

#include "Primitives\Point.h"

namespace jmk 
{
	// Return integer indicating relative position of [Point c] related to segment [a b]
	int pointRelativePos(const Point3d& a, const Point3d& b, const Point3d& c);

	// Predicate to determine whether the [Point c] is left to the segment [a b]
	bool left(const Point3d& a, const Point3d& b, const Point3d& c);

	// Returns counter clockwise angle (0 - 360) measure from referece point to the give point 
	float polarAngle( const Point3d& _other, const Point3d& _ref) ;

	// Return the area of the triangle defined by given 3 points
	double areaTriangle(const Point3d& a, const Point3d& b, const Point3d& c);

	// Return the angle between two vectors
	float angle(const Vector3f& _v1, const Vector3f& _v2);

}


