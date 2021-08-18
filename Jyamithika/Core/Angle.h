#pragma once
#include "Primitives\Line.h"
#include "Primitives\Plane.h"

namespace jmk {
	float AngleLines2D( const Line2d& l1, const Line2d l2);

	float AngleLines3D(const Line& l1, const Line& l2);

	float AngleLinePlane(const Line& l, const Planef p);

	float AnglePlanes(const Planef p1, const Planef p2);
};