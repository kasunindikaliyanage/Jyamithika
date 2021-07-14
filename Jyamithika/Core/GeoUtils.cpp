#include "GeoUtils.h"

#include <math.h>

using namespace jmk;

// This can be only used in 2d XY plane. 
// TODO  has to modify to consider the 3D plane as well
int jmk::pointRelativePos(const Point3d& a, const Point3d& b, const Point3d& c)
{
	float area = areaTriangle2d(a, b, c);
	
	if (area > 0 && area < TOLERANCE)
		area = 0;

	if (area < 0 && area > TOLERANCE)
		area = 0;

	Point3d p1 = b - a;
	Point3d p2 = c - a;

	double p1x, p1y, p2x, p2y;

	p1x = p1[X];
	p1y = p1[Y];
	p2x = p2[X];
	p2y = p2[Y];

	if (area > 0.0)
		return LEFT;
	if (area < 0.0)
		return RIGHT;
	if ((p1x * p2x < 0.0) || (p1y * p2y < 0.0))
		return BEHIND;
	if (p1.magnitude() < p2.magnitude())
		return BEYOND;
	if (a == c)
		return ORIGIN;
	if (b == c)
		return DESTINATION;
	return BETWEEN;

	return 0;
}

bool jmk::left(const Point3d& a, const Point3d& b, const Point3d& c)
{
	return pointRelativePos(a,b,c) == RELATIVE_POSITION::LEFT;
}

bool jmk::right(const Point3d& a, const Point3d& b, const Point3d& c)
{
	return pointRelativePos(a, b, c) == RELATIVE_POSITION::RIGHT;
}

bool jmk::leftOrBeyond(const Point3d& a, const Point3d& b, const Point3d& c)
{
	int position = pointRelativePos(a, b, c);
	return (position == RELATIVE_POSITION::LEFT || position == RELATIVE_POSITION::BEYOND);
}

float jmk::polarAngle( const Point3d& _other, const Point3d& _ref)
{
	if (_other[Z] == 0 && _ref[Z] == 0)
	{
		// Consider the given points as 2D ones which are in XY plane
		float _x = _other[X] - _ref[X];
		float _y = _other[Y] - _ref[Y];

		if ((isEqualD(_x, 0.0)) && (isEqualD(_y, 0.0)))
			return -1.0;
		if (isEqualD(_x, 0.0))
			return ((_y > 0.0) ? 90 : 270);

		double theta = atan(_y / _x);
		theta *= 360 / (2 * M_PI);

		if (_x > 0.0)
			return ((_y >= 0.0) ? theta : 360 + theta);
		else
			return (180 + theta);
	}
	else
	{
		// Points are in 3D space
		return 0.0f;
	}

}

double jmk::areaTriangle2d(const Point3d& a, const Point3d& b, const Point3d& c)
{
	return 0.5 * ((b[X] - a[X]) * (c[Y] - a[Y]) - (c[X] - a[X]) * (b[Y] - a[Y]));
}

double jmk::areaTriangle3d(const Point3d& a, const Point3d& b, const Point3d& c)
{
	float x_, y_, z_;

	Vector3f AB = b - a;
	Vector3f AC = c - a;

	x_ = AB[Y] * AC[Z] - AB[Z] * AC[Y];
	y_ = AB[X] * AC[Z] - AB[Z] * AC[X];
	z_ = AB[X] * AC[Y] - AB[Y] * AC[X];

	float sum_of_powers = pow(x_,2.0) + pow(y_, 2.0) + pow(z_, 2.0);
	float root = sqrtf(sum_of_powers);
	return root / 2;
}

float jmk::angle(const Vector3f& _v1, const Vector3f& _v2)
{
	float dot = dotProduct(_v1, _v2);
	float v1_mag = _v1.magnitude();
	float v2_mag = _v2.magnitude();

	return acos(dot / (v1_mag * v2_mag));
}

bool jmk::isInside(Point3d& _point, std::vector<Point3d>& _points)
{
	return false;
}

int jmk::getClosestPointIndex(Point3d& _point, std::vector<Point3d>& _points)
{
	return 0;
}

bool jmk::collinear(const Point3d& a, const Point3d& b, const Point3d& c)
{
	// No need to calculat the area. Just compare each coeficient vector to zero vec;
	float x_, y_, z_;

	Vector3f AB = b - a;
	Vector3f AC = c - a;

	x_ = AB[Y] * AC[Z] - AB[Z] * AC[Y];
	y_ = AB[X] * AC[Z] - AB[Z] * AC[X];
	z_ = AB[X] * AC[Y] - AB[Y] * AC[X];
	
	return isEqualD(x_, ZERO) && isEqualD(y_, ZERO) && isEqualD(z_, ZERO);
}

bool jmk::coplaner(const Point3d& a, const Point3d& b, const Point3d& c, const Point3d& d)
{
	Vector3f AB = b - a;
	Vector3f AC = c - a;

	float value = scalerTripleProduct(AB, AC, d);
	return isEqualD(value, ZERO);
}
