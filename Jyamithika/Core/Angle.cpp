#include <cmath>

#include "Angle.h"

//float jmk::AngleLines2D( const Line2d& l1, const Line2d l2)
//{
//	auto mag_l1 = l1.direction().magnitude();
//	auto mag_l2 = l2.direction().magnitude();
//	auto dot = dotProduct(l1.direction(), l2.direction());
//
//	auto theta = acos( fabs(dot) / (mag_l1 * mag_l2));
//	return RadianceToDegrees(theta);
//}

//float jmk::AngleLines3D(const Line& l1, const Line& l2)
//{
//	auto mag_l1 = l1.direction().magnitude();
//	auto mag_l2 = l2.direction().magnitude();
//	auto dot = dotProduct(l1.direction(), l2.direction());
//
//	auto theta = acos(fabs(dot) / (mag_l1 * mag_l2));
//	return RadianceToDegrees(theta);
//}

//float jmk::AngleLinePlane(const Line& l, const Planef p)
//{
//	auto dot = dotProduct(l.direction(), p.getNormal());
//	auto theta = acos(fabs(dot));
//	theta = RadianceToDegrees(theta);
//	return 90- theta;
//}

using namespace jmk;

template<class T, size_t dimensions >
static float getAngle(jmk::Vector<T, dimensions> v1, jmk::Vector<T, dimensions> v2)
{
	auto dot = dotProduct(v1, v2);
	auto theta = acos(fabs(dot));
	return RadianceToDegrees(theta);
}

float jmk::AngleLines2D(const Line2d& l1, const Line2d l2)
{
	return getAngle(l1.direction(), l2.direction());
}

float jmk::AngleLines3D(const Line& l1, const Line& l2)
{
	return getAngle(l1.direction(), l2.direction());
}

float jmk::AngleLinePlane(const Line& l, const Planef p)
{
	auto theta = getAngle(l.direction(), p.getNormal());
	return 90 - theta;
}

float jmk::AnglePlanes(const Planef p1, const Planef p2)
{
	return getAngle(p1.getNormal(), p2.getNormal());
}
