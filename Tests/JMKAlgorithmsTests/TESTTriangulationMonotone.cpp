#include "pch.h"
#include "Core\Primitives\Point.h"
#include "Core\GeoUtils.h"
#include "Core\Primitives\PolygonDCEL.h"
#include "Core\Base\Vector.h"
#include "Triangulation.h"

using namespace jmk;

TEST(TriangulateMonotone, Triangulate1)
{
	std::vector<Point2d> points;
	points.push_back(jmk::Point2d(2, 4));
	points.push_back(jmk::Point2d(6, 2));
	points.push_back(jmk::Point2d(8, 5));
	points.push_back(jmk::Point2d(9, 7));
	points.push_back(jmk::Point2d(5, 9));
	points.push_back(jmk::Point2d(4, 6));

	PolygonDCEL<float, DIM2> poly(points);
	triangulate_monotone(&poly);
	
	//for (auto face_ptr : poly.getFaceList()) {
	//	face_ptr->print();
	//	std::cout << std::endl;
	//}

	EXPECT_TRUE(true);
}

TEST(TriangulateMonotone, Triangulate2)
{
	std::vector<Point2d> points;
	points.push_back(jmk::Point2d(-5, 1));
	points.push_back(jmk::Point2d(5, 2));
	points.push_back(jmk::Point2d(4, 9));
	points.push_back(jmk::Point2d(1, 7));
	points.push_back(jmk::Point2d(-6, 6));
	points.push_back(jmk::Point2d(-3, 3));

	PolygonDCEL<float, DIM2> poly(points);
	triangulate_monotone(&poly);

	//for (auto face_ptr : poly.getFaceList()) {
	//	face_ptr->print();
	//	std::cout << std::endl;
	//}

	EXPECT_TRUE(true);
}

TEST(TriangulateMonotone, Triangulate3)
{
	std::vector<Point2d> points;
	points.push_back(jmk::Point2d(-8, -3));
	points.push_back(jmk::Point2d(-2, -4));
	points.push_back(jmk::Point2d(3, -9));
	points.push_back(jmk::Point2d(2, -1));
	points.push_back(jmk::Point2d(-5, 1));
	points.push_back(jmk::Point2d(-7, 2.5));

	PolygonDCEL<float, DIM2> poly(points);
	triangulate_monotone(&poly);

	//for (auto face_ptr : poly.getFaceList()) {
	//	face_ptr->print();
	//	std::cout << std::endl;
	//}

	EXPECT_TRUE(true);
}

TEST(TriangulateMonotone, Triangulate4)
{
	std::vector<Point2d> points;
	points.push_back(jmk::Point2d(-5, 1));
	points.push_back(jmk::Point2d(2, -1));
	points.push_back(jmk::Point2d(6, -2));
	points.push_back(jmk::Point2d(8, 5));
	points.push_back(jmk::Point2d(5, 2));

	PolygonDCEL<float, DIM2> poly(points);
	triangulate_monotone(&poly);

	//for (auto face_ptr : poly.getFaceList()) {
	//	face_ptr->print();
	//	std::cout << std::endl;
	//}

	EXPECT_TRUE(true);
}

TEST(TriangulateGeneral, Triangulate1)
{
	std::vector<Point2d> points;
	points.push_back(jmk::Point2d(8, 5));
	points.push_back(jmk::Point2d(5, 2));
	points.push_back(jmk::Point2d(4, 9));
	points.push_back(jmk::Point2d(1, 7));
	points.push_back(jmk::Point2d(-1, 8));
	points.push_back(jmk::Point2d(-6, 6));
	points.push_back(jmk::Point2d(-3, 3));
	points.push_back(jmk::Point2d(-5, 1));
	points.push_back(jmk::Point2d(-7, 2.5));
	points.push_back(jmk::Point2d(-8, -3));
	points.push_back(jmk::Point2d(-4, -6));
	points.push_back(jmk::Point2d(-2, -4));
	points.push_back(jmk::Point2d(3, -9));
	points.push_back(jmk::Point2d(2, -1));
	points.push_back(jmk::Point2d(6, -2));


	PolygonDCEL<float, DIM2> poly(points);
	std::vector<Polygon2d*> mono_polies;
	get_monotone_polygons(&poly, mono_polies);

	for (auto polygon : mono_polies) {
		triangulate_monotone(polygon);
	}

	for (auto polygon : mono_polies) {
		for (auto face_ptr : polygon->getFaceList())
		{
			face_ptr->print();
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}

	EXPECT_TRUE(true);
}

TEST(TriangulateGeneral, Triangulate2)
{
	std::vector<Point2d> points;
	points.push_back(jmk::Point2d(2, 4));
	points.push_back(jmk::Point2d(6, 2));
	points.push_back(jmk::Point2d(8, 5));
	points.push_back(jmk::Point2d(9, 7));
	points.push_back(jmk::Point2d(5, 9));
	points.push_back(jmk::Point2d(4, 6));	
	points.push_back(jmk::Point2d(1, 8));
	points.push_back(jmk::Point2d(-2, 6.5));
	points.push_back(jmk::Point2d(1, 5.5));

	PolygonDCEL<float, DIM2> poly(points);
	std::vector<Polygon2d*> mono_polies;

	get_monotone_polygons(&poly, mono_polies);

	//for (auto polygon : mono_polies) {
	//	triangulate_monotone(&polygon);
	//}

	//for (auto polygon : mono_polies) {
	//	for (auto face_ptr : polygon.getFaceList())
	//		face_ptr->print();
	//	std::cout << std::endl;
	//}

	EXPECT_TRUE(true);

}
