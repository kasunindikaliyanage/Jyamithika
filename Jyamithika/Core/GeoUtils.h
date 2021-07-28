#pragma once

#include <vector>
#include "Primitives\Point.h"
#include "Primitives\Plane.h"
#include "Primitives\Segment.h"
#include "Primitives\Polygon.h"
#include "Primitives\Polyhedron.h"

namespace jmk 
{
	// Return integer indicating relative position of [Point c] related to segment [a b]
	// This is only for 2D in XY plane.
	int pointRelativePos(const Point3d& a, const Point3d& b, const Point3d& c);

	// Predicate to determine whether the [Point c] is left to the segment [a b]
	bool left(const Point3d& a, const Point3d& b, const Point3d& c);

	// Predicate to determine whether the [Point c] is left to the segment [a b]
	bool left(const Point2d& a, const Point2d& b, const Point2d& c);

	// Predicate to determine whether the [Point c] is right to the segment [a b]
	bool right(const Point3d& a, const Point3d& b, const Point3d& c);

	// Predicate to determine whether the [Point c] is left to the segment [a b]
	bool leftOrBeyond(const Point3d& a, const Point3d& b, const Point3d& c);

	// Predicate to determine whether the [Point c] is left to or between the segment [a b]
	bool leftOrBetween(const Point3d& a, const Point3d& b, const Point3d& c);

	// Returns counter clockwise angle (0 - 360) measure from referece point to the give point 
	float polarAngle( const Point3d& _other, const Point3d& _ref) ;

	// Return the area of the triangle defined by given 3 points
	// TODO change the parameter types form 3d to 2d
	double areaTriangle2d(const Point3d& a, const Point3d& b, const Point3d& c);

	// Return the area of the triangle defined by given 3 points
	double areaTriangle3d(const Point3d& a, const Point3d& b, const Point3d& c);
	
	float volume(const Point3d& a, const Face& f );

	// Return the orientation of the points in the Face f. CW or CCW
	int orientation(const Face& f, const Vector3f& p);

	// Return the signed area of the tetrahedron.
	// Assume the face vertices are in CCW order;
	float volumeSigned(const Face& _f, const Point3d& _p);

	// Return the angle between two vectors
	float angle(const Vector3f& _v1, const Vector3f& _v2);

	// Return whether the [point] is inside the polygon represented by [_points].
	// Note that the check assume that [_points] are in counter clockwise order
	// TODO : Better if we can check this on polygon
	bool isInside(Point3d& _point, std::vector<Point3d>& _points);

	// Return whether the [point] is inside the polyhedron represented by [_faces] list
	bool isInside(Point3d& _point, std::vector<Face>& _faces);

	// Return the index of the point closest to the [_point] from [_points] 
	// TODO : Better if we can check this on polygon
	int getClosestPointIndex(Point3d& _point, std::vector<Point3d>& _points);

	// Check the collinearity of given three points in 3D.
	// Three points are colinear if the coeficient taken by determinant is zero.
	// No need to calculate area.(Calculation of area could be much slower as we have pow and sqrt)
	bool collinear(const Point3d& a, const Point3d& b, const Point3d& c);

	// Check the coplaness of given four points in 3D.
	// The three vectors are coplanar if their scalar triple product is zero.
	bool coplaner(const Point3d& a, const Point3d& b, const Point3d& c, const Point3d& d);

	// Check whether the given 3 vectors are coplaner
	// Vectors parallel to the same plane, or lie on the same plane are called coplanar
	bool coplaner(const Vector3f& _v1, const Vector3f& _v2, const Vector3f& _v3);

	bool segmentIsLeft(const Segment2d& base, const Segment2d& compare, const Point2d& _point);

	Vector3f getFaceNormal(const Point3d& a, const Point3d& b, const Point3d& c);

	float volumTetrahedron(const Point3d& a, const Point3d& b, const Point3d& c, const Point3d& d);

	void get_monotone_polygons(Polygon2d* poly, std::vector<Polygon2d>& mono_polies);
}


