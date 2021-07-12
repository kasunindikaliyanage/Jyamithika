#pragma once

#include <vector>
#include "Core\Primitives\Point.h"
#include "Core\Primitives\Polygon.h"

namespace jmk 
{
	// Note : All the 2D algorithms assume the provided points are in single plane. If not the result could be wrong

	// Compute the points in the convex hull. 
	// Assumptions : (Preprocess the points sets to satisfy following assumptions)
	//				 The points are in XY 2D plane.
	//				 No duplicate points.
	void convexhull2DGiftwrapping(std::vector<Point3d>& _points, std::vector<Point3d>& _convex);
	
	// Compute the points in the convex hull. Assume the points are in XY 2D plane.
	// Note : Order of the [_points] will be changed due to internal sort calls
	// Assumptions : (Preprocess the points sets to satisfy following assumptions)
	//				 The points are in XY 2D plane.
	//				 No duplicate points.
	void convexhull2DModifiedGrahams(std::vector<Point3d>& _points, std::vector<Point3d>& _convex);

	// Compute the points in the convex hull in incremental way. Assume the points are in XY 2D plane.
	// Pre order the points from left to right to reduce the time
	// Explain why we need pre sorting 
	void convexhull2DIncremental(std::vector<Point3d>& _points, std::vector<Point3d>& _convex);

	// Compute the points in the convex hull by following divide and conquire methodology.
	// Assume the points are in XY 2D plane.
	void convexhull2DDivideAndConquer(std::vector<Point3d>& _points, Polygon& _results);

	// Compute the points in the convex hull in 3D space using incremental methodology.
	void convexhull3D(std::vector<Point3d>& _points, std::vector<Point3d>& _convex);
}