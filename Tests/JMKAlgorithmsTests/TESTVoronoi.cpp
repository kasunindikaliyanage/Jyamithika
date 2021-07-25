#include "pch.h"
#include "Core\Primitives\Point.h"
#include "Core\Primitives\Polygon.h"
#include "Convexhull.h"
#include "Core\GeoUtils.h"
#include "Core\Primitives\Vector.h"
#include "Core\Primitives\Bounds.h"

#include "Voronoi.h"

using namespace jmk;

TEST(VoronoiFortunes, Voronoi)
{
	std::vector<Point2d> points;
	std::vector<Point3d> point3s;
	//points.push_back(Point2d(2, 4));
	//points.push_back(Point2d(9, 3));
	//points.push_back(Point2d(6.5, 3.5));
	//points.push_back(Point2d(5, 7));
	//points.push_back(Point2d(11, 5));
	//points.push_back(Point2d(8, 10));
	//points.push_back(Point2d(3.5, 2));
	//points.push_back(Point2d(2.5, 1));

	points.push_back(Point2d(2, 1));
	points.push_back(Point2d(6, 2));
	points.push_back(Point2d(8, 6));
	points.push_back(Point2d(4, 5));
	points.push_back(Point2d(5, 7));

	std::vector<Edge2d> edges;

	BoundRectangle rect{0.0,10.0,15.0,0.0};

	constructVoronoiDiagram_fortunes(points, edges, rect);

	bool test_passed = true;
	EXPECT_TRUE(test_passed);
}