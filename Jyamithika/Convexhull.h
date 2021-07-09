#pragma once

#include <vector>
#include "Core\Primitives\Point.h"
#include "Core\Primitives\Polygon.h"

namespace jmk 
{
	// Note : All the 2D algorithms assume the provided points are in single plane. If not the result could be wrong

	// Compute the points in the convex hull. Assume the points are in XY 2D plane.
	void convexhull2DGiftwrapping(std::vector<Point3d>& _points, std::vector<Point3d>& _convex);
	
	void convexhull2DGrahams(std::vector<Point3d>& _points, std::vector<Point3d>& _convex);

	void convexhull2DGrahams_modified(std::vector<Point2d>& _points, std::vector<Point3d>& _convex);

	void convexhull3D(std::vector<Point3d>& _points, std::vector<Point3d>& _convex);

}