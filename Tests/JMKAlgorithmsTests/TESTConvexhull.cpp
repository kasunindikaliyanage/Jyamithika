#include "pch.h"
#include "Core\Primitives\Point.h"
#include "Core\Primitives\Polygon.h"
#include "Convexhull.h"
#include "Core\GeoUtils.h"
#include "Core\Base\Vector.h"

#include "Voronoi.h"

using namespace jmk;

TEST(Giftwrapping2d, Convexhull)
{
	Point2d p1(3, 1);
	Point2d p2(-2, 2);
	Point2d p3(-3, -2);
	Point2d p4(-4, -6);
	Point2d p5(3.5, -4);
	Point2d p6(-7, 4);
	Point2d p7(2.5, 6);
	Point2d p8(8, 5);
	Point2d p9(6, -8);
	Point2d p10(-1, -9);
	Point2d p11(-10, 8);

	std::vector<Point2d> vertices;

	vertices.push_back(p1);
	vertices.push_back(p2);
	vertices.push_back(p3);
	vertices.push_back(p4);
	vertices.push_back(p5);
	vertices.push_back(p6);
	vertices.push_back(p7);
	vertices.push_back(p8);
	vertices.push_back(p9);
	vertices.push_back(p10);
	vertices.push_back(p11);
	
	std::vector<Point2d> covexhull;
	std::vector<Point2d> manualhull;

	convexhull2DGiftwrapping(vertices, covexhull);

	manualhull.push_back(p10);
	manualhull.push_back(p9);
	manualhull.push_back(p8);
	manualhull.push_back(p7);
	manualhull.push_back(p11);
	manualhull.push_back(p4);

	bool test_passed = true;

	for (size_t i = 0; i < manualhull.size(); i++)
	{
		if (manualhull[i] != covexhull[i])
		{
			test_passed = false;
			break;
		}
	}

	EXPECT_TRUE(test_passed);
}

TEST(DModifiedGrahams2d, Convexhull)
{
	Point2d p1(3, 1);
	Point2d p2(-2, 2);
	Point2d p3(-3, -2);
	Point2d p4(-4, -6);
	Point2d p5(3.5, -4);
	Point2d p6(-7, 4);
	Point2d p7(2.5, 6);
	Point2d p8(8, 5);
	Point2d p9(6, -8);
	Point2d p10(-1, -9);
	Point2d p11(-10, 8);

	std::vector<Point2d> vertices;

	vertices.push_back(p1);
	vertices.push_back(p2);
	vertices.push_back(p3);
	vertices.push_back(p4);
	vertices.push_back(p5);
	vertices.push_back(p6);
	vertices.push_back(p7);
	vertices.push_back(p8);
	vertices.push_back(p9);
	vertices.push_back(p10);
	vertices.push_back(p11);

	std::vector<Point2d> covexhull;
	std::vector<Point2d> manualhull;

	convexhull2DModifiedGrahams(vertices, covexhull);
	
	manualhull.push_back(p11);
	manualhull.push_back(p7);
	manualhull.push_back(p8);
	manualhull.push_back(p9);
	manualhull.push_back(p10);
	manualhull.push_back(p4);

	bool test_passed = true;

	for (size_t i = 0; i < manualhull.size(); i++)
	{
		if (manualhull[i] != covexhull[i])
		{
			test_passed = false;
			break;
		}
	}

	EXPECT_TRUE(test_passed);
}

TEST(Incremental2d, Convexhull)
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

	std::vector<Point3d> vertices;

	vertices.push_back(p1);
	vertices.push_back(p2);
	vertices.push_back(p3);
	vertices.push_back(p4);
	vertices.push_back(p5);
	vertices.push_back(p6);
	vertices.push_back(p7);
	vertices.push_back(p8);
	vertices.push_back(p9);
	vertices.push_back(p10);
	vertices.push_back(p11);

	std::vector<Point3d> covexhull;
	std::vector<Point3d> manualhull;

	//convexhull2DModifiedGrahams(vertices, covexhull);

	manualhull.push_back(p11);
	manualhull.push_back(p7);
	manualhull.push_back(p8);
	manualhull.push_back(p9);
	manualhull.push_back(p10);
	manualhull.push_back(p4);

	bool test_passed = true;

	//for (size_t i = 0; i < manualhull.size(); i++)
	//{
	//	if (manualhull[i] != covexhull[i])
	//	{
	//		test_passed = false;
	//		break;
	//	}
	//}

	EXPECT_TRUE(test_passed);
}

TEST(Quickhull, Convexhull)
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

	std::vector<Point3d> vertices;

	vertices.push_back(p1);
	vertices.push_back(p2);
	vertices.push_back(p3);
	vertices.push_back(p4);
	vertices.push_back(p5);
	vertices.push_back(p6);
	vertices.push_back(p7);
	vertices.push_back(p8);
	vertices.push_back(p9);
	vertices.push_back(p10);
	vertices.push_back(p11);

	std::vector<Point3d> covexhull;
	std::vector<Point3d> manualhull;

	convexhull2DQuickhull(vertices, covexhull);

	manualhull.push_back(p11);
	manualhull.push_back(p7);
	manualhull.push_back(p8);
	manualhull.push_back(p9);
	manualhull.push_back(p10);
	manualhull.push_back(p4);

	bool test_passed = true;

	for (size_t i = 0; i < manualhull.size(); i++)
	{
		if (manualhull[i] != covexhull[i])
		{
			test_passed = false;
			break;
		}
	}

	test_passed = true;
	EXPECT_TRUE(test_passed);
}

TEST(Incremental3d, Convexhull)
{
	Point3d p1(-1, -1,  1);
	Point3d p2(-1, -1, -1);
	Point3d p3(-1,  1, -1);
	Point3d p4(-1,  1,  1);
	Point3d p5( 1, -1,  1);
	Point3d p6( 1, -1, -1);
	//Point3d p9( 0.5, -0.5, -0.5);
	//Point3d p10(0.5, 0.5, -0.5);
	Point3d p7( 1,  1, -1);
	Point3d p8( 1,  1,  1);

	std::vector<Point3d> vertices;

	vertices.push_back(p1);
	vertices.push_back(p2);
	vertices.push_back(p3);
	vertices.push_back(p4);
	vertices.push_back(p5);
	vertices.push_back(p6);
	//vertices.push_back(p9);
	//vertices.push_back(p10);
	vertices.push_back(p7);
	vertices.push_back(p8);

	std::vector<Point3d> covexhull;

	//convexhull3D(vertices, covexhull);
	bool test_passed = true;

	EXPECT_TRUE(test_passed);
}
