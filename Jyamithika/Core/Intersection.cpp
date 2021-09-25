#include "Base\Core.h"
#include "Intersection.h"
#include "GeoUtils.h"

using namespace jmk;

bool jmk::intersect(jmk::Line2d& l1, jmk::Line2d& l2, jmk::Point2d& pi){
	Vector2f l1p = l1.point();
	Vector2f l2p = l2.point();
	Vector2f l1d = l1.direction();
	Vector2f l2d = l2.direction();

	float a, b, c, d, e, f;

	a = l1d[X];
	b = -l2d[X];
	c = l2p[X] - l1p[X];
	d = l1d[Y];
	e = -l2d[Y];
	f = l2p[Y] - l1p[Y];

	Vector2f diff = l2p - l1p;
	auto prep_l2d = prependicluar(l2d);
	float dot_d0_prepd1 = dotProduct(l1d, prep_l2d);

	if (!isEqualD(dot_d0_prepd1, ZERO))
	{
		float denominator = a * e - b * d;
		float t_numerator = c * e - b * f;
		float s_numerator = a * f - c * a;

		float t = t_numerator / denominator;

		float x = l1p[X] + t * l1d[X];
		float y = l1p[Y] + t * l1d[Y];

		if ((x - l1p[X]) / l1d[X] < 0) return false;
		if ((y - l1p[Y]) / l1d[Y] < 0) return false;

		if ((x - l2p[X]) / l2d[X] < 0) return false;
		if ((y - l2p[Y]) / l2d[Y] < 0) return false;
	
		pi.assign(X, x);
		pi.assign(Y, y);
		return true;
	}
	else
	{
		//Lines are parallel
		return false;
	}
}

bool jmk::intersect(const jmk::Point2d& a, const jmk::Point2d& b, const jmk::Point2d& c, const jmk::Point2d& d) {
	
	// if one of the end points of a segment is in between other segment endpoints we consider it as intersection.
	if (jmk::orientation2d(a, b, c) == jmk::BETWEEN 
		|| jmk::orientation2d(a, b, d) == jmk::BETWEEN 
		|| jmk::orientation2d(c, d, a) == jmk::BETWEEN 
		|| jmk::orientation2d(c, d, b) == jmk::BETWEEN )
	{
		return true;
	}

	return jmk::_xor(jmk::left(a,b,c), jmk::left(a,b,d)) && jmk::_xor(jmk::left(c, d, a), jmk::left(c, d, b));
}

bool jmk::intersect(jmk::Point2d& a, jmk::Point2d& b , jmk::Point2d& c , jmk::Point2d& d , jmk::Point2d& interseciton)
{
	Vector2f AB = b - a;
	Vector2f CD = d - c;

	//Normal vector to CD
	Vector2f n( CD[Y], -CD[X]);

	//Denominator = n.(b-a)
	auto deno = dotProduct(n, AB);

	if (!isEqualD(deno, ZERO))
	{
		auto AC = c-a;
		auto nume = dotProduct(n, AC);
		auto t = nume / deno;

		auto x = a[X] + t * AB[X];
		auto y = a[Y] + t * AB[Y];

		interseciton.assign(X, x);
		interseciton.assign(Y, y);
		return true;
	}
	else
	{
		// Lines are parallel or colinear
		return false;
	}
}

bool jmk::intersect(jmk::Planef& plane, jmk::Line& line, jmk::Point3d& point){
	
	auto n = plane.getNormal();
	auto D = plane.getD();
	auto d = line.direction();
	auto p = line.point();

	auto denominator = dotProduct(n, d);

	if (!isEqualD(denominator, ZERO)){
		auto t = (-1 * dotProduct(n, p) + D) / denominator;
		point.assign(X, p[X] + t * d[X]);
		point.assign(Y, p[Y] + t * d[Y]);
		point.assign(Z, p[Z] + t * d[Z]);
		return true;
	}
	else{
		return false;
	}
}

bool jmk::intersect(jmk::Planef& p1, jmk::Planef& p2, jmk::Line& l) {
	
	auto n1 = p1.getNormal();
	auto n2 = p2.getNormal();
	auto d1 = p1.getD();
	auto d2 = p2.getD();

	auto direction = crossProduct3d(n1,n2);
	direction.normalize();

	// Check if the planes are parallel.
	if (isEqualD(direction.magnitude(), ZERO))
		return false;

	auto n1n2 = dotProduct(n1, n2);
	auto n1n2_2 = n1n2 * n1n2;

	auto a = (d2 * n1n2 - d1) / (n1n2_2 - 1);
	auto b = (d1 * n1n2 - d2) / (n1n2_2 - 1);

	auto point = n1*a + n2*b;
	
	l.setPoint(point);
	l.setDirection(direction);

	return true;
}

bool jmk::intersect(jmk::Line2d& line, jmk::Segment2d& seg) {
	
	// TODO : This can be done efficiently by checking the end points orientation
	// compared to the line.
	Vector2f dir = seg.p2 - seg.p1;
	Point2d point = seg.p1;
	jmk::Line2d seg_line( point, dir );
	return intersect(line, seg_line, point);
}

bool jmk::intersect(jmk::Line2d& line, jmk::Segment2d& seg, jmk::Point2d& pi) {
	Vector2f dir = seg.p2 - seg.p1;
	Point2d point = seg.p1;
	jmk::Line2d seg_line(point, dir);
	return intersect(line, seg_line, pi);
}