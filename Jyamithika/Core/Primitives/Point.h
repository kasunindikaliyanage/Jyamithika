#pragma once

#include "Vector.h"

namespace jmk {

	typedef Vector2f Point2d;
	typedef Vector3f Point3d;

	bool sort2DLRTB(const Point3d& a, const Point3d& b);

	bool sort3DTBLR(const Point3d& a, const Point3d& b);

	bool sort2DTBLR(const Point2d& a, const Point2d& b);

}