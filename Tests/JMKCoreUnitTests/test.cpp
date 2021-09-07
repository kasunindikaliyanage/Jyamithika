#include "pch.h"
#include "Core\Base\Core.h"
#include "Core\Primitives\Point.h"
#include "Core\GeoUtils.h"
#include "Core\Primitives\PolygonDCEL.h"
#include "Core\Base\Vector.h"
#include "Core\Primitives\Line.h"
#include "Core\Distance.h"
#include "Core\Intersection.h"
#include "Core\Angle.h"
#include "Core\Intersection.h"

using namespace jmk;

const Point3d origin3d(0.0, 0.0, 0.0);
const Point2d origin2d(0.0, 0.0);

TEST(CrossProduct, test1)
{
	Vector3f pNormal1(10, 2, 3);
	Vector3f pNormal2(2, 15, 2);

	pNormal1.normalize();
	pNormal2.normalize();

	auto result = crossProduct3d(pNormal1, pNormal2);
	result.normalize();
	EXPECT_TRUE(true);
}

TEST(PointIn_1st_QuadToOrigin2D, PolarAngle)
{
	Point2d check(4.0, 3.5);
	double angle = polarAngle(check, origin2d);
	EXPECT_TRUE(isEqualDL(41.18592, angle));
}

TEST(PointIn_2nd_QuadToOrigin2D, PolarAngle)
{
	Point2d check(-4.0, 3.5);
	double angle = polarAngle(check, origin2d);
	EXPECT_TRUE(isEqualDL(138.81407, angle));
}

TEST(PointIn_3rd_QuadToOrigin2D, PolarAngle)
{
	Point2d check(-4.0, -3.5);
	double angle = polarAngle(check, origin2d);
	EXPECT_TRUE(isEqualDL(221.18592, angle));
}

TEST(PointIn_4th_QuadToOrigin2D, PolarAngle)
{
	Point2d check(4.0, -3.5);
	double angle = polarAngle(check, origin2d);
	EXPECT_TRUE(isEqualDL(318.81408, angle));
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

TEST(distancePointLine, DistanceTest)
{
	Point3d point_ref(-1, -9, 1);
	//Point3d point_ref(-0.34, 0, 0);
	Point3d p1(-10, 8, 2);
	Point3d p2(8, 5, -3);
	Line l1(p1, p2);

	float distance1 = distance(l1,point_ref);
	float distance2 = distance(p1,p2, point_ref);

	EXPECT_TRUE(isEqualDL(distance1,distance2));
}

TEST(increment, increTest)
{
	std::list<float> values;
	values.push_back(1.0);
	values.push_back(1.0);
	values.push_back(1.0);

	std::list<float>::iterator itr = values.begin();
	std::advance(itr, 2);

	if (itr!= values.begin())
	{
		std::cout << "Check 1" << std::endl;
	}
	{
		std::cout << "Check 2" << std::endl;
	}

}

TEST(LineIntersection, intersection)
{
	Point2d p1, p2 , p_intersect;
	p1.assign(X,5);
	p1.assign(Y, 10);
	p2.assign(X,11);
	p2.assign(Y, 8.4);

	Vector2f v1, v2;
	v1.assign(X, 3);
	v1.assign(Y,-3);
	v2.assign(X,-5);
	v2.assign(Y, -3);

	Line2d l1(p1, v1);
	Line2d l2(p2, v2);

	bool result = intersect(l1, l2, p_intersect);

	EXPECT_TRUE(result);
}

TEST(DCELConstructionSplit,DCELTest)
{
	std::vector<Point2d> points;
	points.push_back(jmk::Point2d(2 ,4));
	points.push_back(jmk::Point2d(6, 2));
	points.push_back(jmk::Point2d(8,5));
	points.push_back(jmk::Point2d(9, 7));
	points.push_back(jmk::Point2d(5,9));
	points.push_back(jmk::Point2d(4, 6));
	points.push_back(jmk::Point2d(1,8));
	points.push_back(jmk::Point2d(-2, 6.5));
	points.push_back(jmk::Point2d(0,6.5));
	points.push_back(jmk::Point2d(1, 4.5));

	PolygonDCEL<float, DIM2> poly(points);

	for (auto edge : poly.getEdgeList()){
		edge->print();
		std::cout << "\n";
	}
	auto vertex1 = poly.getVertex(jmk::Point2d(1, 4.5));
	auto vertex2 = poly.getVertex(jmk::Point2d(4, 6));

	poly.split( vertex1, vertex2);

	for (auto face_ptr : poly.getFaceList())
	{
		face_ptr->print();
		std::cout << std::endl;
	}
	EXPECT_TRUE(true);
}

TEST(AngleTest, AngleLines2DTest1 )
{
	Vector2f l1Dir(2, 1);
	Vector2f l2Dir(-2, 1);
	Point2d l1P(0, 3);
	Point2d l2P(0,5);

	Line2d l1(l1P, l1Dir);
	Line2d l2(l2P, l2Dir);

	auto result = AngleLines2D(l1, l2);

	EXPECT_TRUE(isEqualDLL(53.1301023, result));
}

TEST(AngleTest, AngleLines3DTest1)
{
	Point3d l1P1(1, 5, 5);
	Point3d l1P2(1.5, -5, -2);
	Point3d l2P1(-3, -3, 0);
	Point3d l2P2(3, 5, 2);


	Line l1(l1P1,l1P2);
	Line l2(l2P1, l2P2);

	auto result = AngleLines3D(l1, l2);

	EXPECT_TRUE(isEqualDLL(43.08, result));
}

TEST(AngleTest, AngleLinePlaneTest1)
{
	Point3d l1P1(-3, -4, -5);
	Point3d l1P2(4, 3, 5);
	
	Vector3f pNormal(-14.26,9.32,18.89);

	Line l1(l1P1, l1P2);
	Planef p(pNormal);

	auto result = AngleLinePlane(l1, p);

	EXPECT_TRUE(isEqualDLL(25.55, result));
}

TEST(AngleTest, AnglePlanesTest1)
{
	Vector3f pNormal1(1.68, 0.42, 2.35);
	Vector3f pNormal2(10.47, -4.44, -4.96);

	Planef p1(pNormal1);
	Planef p2(pNormal2);


	auto result = AnglePlanes(p1,p2);

	EXPECT_TRUE(isEqualDLL(83.549, result));
}

TEST(IntersectionTest, TwoPlanes1)
{
	Vector3f pNormal1(1, 2, 3);
	Vector3f pNormal2(2, 5, -2);

	
	auto c1 = 6 / pNormal1.magnitude();
	auto c2 = -4 / pNormal2.magnitude();

	Planef p1(pNormal1,  c1);
	Planef p2(pNormal2, c2);

	Line l1;

	auto result = intersect(p1, p2, l1);

	EXPECT_TRUE(isEqualDLL(83.549, result));
}

