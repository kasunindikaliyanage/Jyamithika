#include "pch.h"
#include "Core\Utils\Core.h"
#include "Core\Primitives\Point.h"
#include "Core\GeoUtils.h"
#include "Core\Primitives\Vector.h"

using namespace jmk;

const Point3d origin(0.0, 0.0, 0.0);

TEST(PointIn_1st_QuadToOrigin2D, PolarAngle)
{
	Point3d check(4.0, 3.5);
	double angle = polarAngle(check, origin);
	EXPECT_TRUE(isEqualD(41.18592, angle));
}

TEST(PointIn_2nd_QuadToOrigin2D, PolarAngle)
{
	Point3d check(-4.0, 3.5);
	double angle = polarAngle(check, origin);
	EXPECT_TRUE(isEqualD(138.81407, angle));
}

TEST(PointIn_3rd_QuadToOrigin2D, PolarAngle)
{
	Point3d check(-4.0, -3.5);
	double angle = polarAngle(check, origin);
	EXPECT_TRUE(isEqualD(221.18592, angle));
}

TEST(PointIn_4th_QuadToOrigin2D, PolarAngle)
{
	Point3d check(4.0, -3.5);
	double angle = polarAngle(check, origin);
	EXPECT_TRUE(isEqualD(318.81408, angle));
}
