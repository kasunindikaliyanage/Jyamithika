#include "pch.h"
#include "Core\Primitives\Point.h"
#include "Core\Primitives\Polygon.h"
#include "Convexhull.h"
#include "Core\GeoUtils.h"
#include "Core\Primitives\Vector.h"

using namespace jmk;

TEST(Giftwrapping, Convexhull)
{
	Point3d p1(3, 1);
	Point3d p2(-2, 2);
	Point3d p3(-3, -2);
	Point3d p4(-4, -6);
	Point3d p5(3.5, -4);
	Point3d p6(-7, 4);
	Point3d p7(2.5, 6);
	Point3d p8(8, 5);
	Point3d p9(6, -8);
	Point3d p10(-1, -9);
	Point3d p11(-10, 8);

	std::vector<Point3d> points;

	points.push_back(p1);
	points.push_back(p2);
	points.push_back(p3);
	points.push_back(p4);
	points.push_back(p5);
	points.push_back(p6);
	points.push_back(p7);
	points.push_back(p8);
	points.push_back(p9);
	points.push_back(p10);
	points.push_back(p11);
	
	std::vector<Point3d> covexhull;

	convexhull2DGiftwrapping(points, covexhull);
	std::cout << "Test";

	EXPECT_TRUE(true);
}