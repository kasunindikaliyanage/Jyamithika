#pragma once

#include <vector>
#include "Core\Primitives\Point.h"
#include "Core\Primitives\Polygon.h"

namespace jmk 
{
	// Note : All the 2D algorithms assume the provided points are in single plane. If not the result could be wrong

	void convexhull2dGiftwrapping(std::vector<Point3d>& _points, Polygon& _polygon);
	
	void convexhull2dGrahams(std::vector<Point2d>& _points, Polygon& _polygon);

	void convexhull2dGrahams_modified(std::vector<Point2d>& _points, Polygon& _polygon);

	void convexhull3d(std::vector<Point3d>& _points, Polygon& _polygon);

}