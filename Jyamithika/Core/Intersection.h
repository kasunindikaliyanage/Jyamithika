#pragma once
// Include functions to calculate intersection between different geometric promitives
#include "Primitives\Point.h"
#include "Primitives\Line.h"
#include "Primitives\Plane.h"
#include "Primitives\Segment.h"
#include "Primitives\Polygon.h"
#include "Primitives\Polyhedron.h"

namespace jmk {

	bool intersect(jmk::Line&, jmk::Line&, jmk::Point3d&);
	
	// Return true if two 2d lines are intersecting and store the intersection point in final argument
	// 1 - First line
	// 2 - Second line
	bool intersect(jmk::Line2d&,jmk::Line2d&, jmk::Point2d&);

	bool intersect(jmk::Line2d&, jmk::Segment2d&);

	bool intersect(jmk::Line2d&, jmk::Segment2d&, jmk::Point2d&);

	//bool intersect(jmk::Segment2d&, jmk::Segment2d&, jmk::Point2d&);

	// Return true if 2d segments represeted by points are intersecting
	// 1 - start point of first segment
	// 2 - end point of first segmenet
	// 3 - start point of second segment
	// 4 - end point of second segment
	bool intersect(const jmk::Point2d&, const jmk::Point2d&, const jmk::Point2d&, const jmk::Point2d&);

	//// Return true if 2d segments represeted by points are intersecting. Store the intersecting point in final argument
	//// 1 - start point of first segment
	//// 2 - end point of first segmenet
	//// 3 - start point of second segment
	//// 4 - end point of second segment
	//// 5 - intersection point
	bool intersect(jmk::Point2d&, jmk::Point2d&, jmk::Point2d&, jmk::Point2d&, jmk::Point2d&);

	// Return true if Plane and Line are intersecting. Intersection point will be stored point pass as 3rd argument
	bool intersect(jmk::Planef&, jmk::Line&, jmk::Point3d&);

	// Return true if the given two planes are intersecting. Store the line of intersect in the line pass as 3rd argument
	bool intersect(jmk::Planef&, jmk::Planef&, jmk::Line&);
}