#include "pch.h"
#include "Core\Base\Core.h"
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

TEST(OrientationTest1, OrientationTest)
{
	Point3d point_ref(-0.8, 0.26, -0.57);
	//Point3d point_ref(-0.34, 0, 0);
	Point3d p1(-1,-1,-1);
	Point3d p2(-1,1,-1);
	Point3d p3(-1,1,1);

	Vertex3d v1(&p1);
	Vertex3d v2(&p2);
	Vertex3d v3(&p3);

	Face f( &v1,&v2,&v3);
	float order = orientation(f, point_ref);
	EXPECT_TRUE(order == 2);
}

TEST(OrientationTest2, OrientationTest)
{
	Point3d point_ref(-1.8, 0.26, -0.57);
	//Point3d point_ref(-0.34, 0, 0);
	Point3d p1(-1, -1, -1);
	Point3d p2(-1, 1, -1);
	Point3d p3(-1, 1, 1);

	Vertex3d v1(&p1);
	Vertex3d v2(&p2);
	Vertex3d v3(&p3);

	Face f(&v1, &v2, &v3);
	float order = orientation(f, point_ref);
	EXPECT_TRUE(order == 1);
}