#pragma once

#include "../Base/Vector.h"

namespace jmk {

	typedef Vector2f Point2d;
	typedef Vector3f Point3d;

	static const Point2d DEFAULT_POINT_2D(FLT_MAX, FLT_MAX);
	static const Point3d DEFAULT_POINT_3D(FLT_MAX, FLT_MAX, FLT_MAX);

	bool sort2DLRTB(const Point2d& a, const Point2d& b);

	bool sort3DTBLR(const Point3d& a, const Point3d& b);

	bool sort2DTBLR(const Point2d& a, const Point2d& b);

	bool sort2DbyX(const Point2d& a, const Point2d& b);

	bool sort2DbyY(const Point2d& a, const Point2d& b);

}