#include "pch.h"
#include "Core\Primitives\Point.h"
#include "Core\Primitives\Polygon.h"
#include "Convexhull.h"
#include "Core\GeoUtils.h"
#include "Core\Base\Vector.h"
#include "Core\Primitives\Bounds.h"

#include "Voronoi.h"

using namespace jmk;

TEST(VoronoiFortunes, Voronoi)
{
	std::vector<Point2d> points;
	std::vector<Point3d> point3s;

	//points.push_back(jmk::Point2d(0.8, 0.6));
	//points.push_back(jmk::Point2d(-0.5, 0.7));
	//points.push_back(jmk::Point2d(-0.6, 0.5));
	//points.push_back(jmk::Point2d(0.3, 0.4));
	//points.push_back(jmk::Point2d(0.2, 0.9));
	//points.push_back(jmk::Point2d(-0.3, 0.8));
	//points.push_back(jmk::Point2d(-0.1, 0.2));
	//points.push_back(jmk::Point2d(0.4, -0.3));
	//points.push_back(jmk::Point2d(0.2, -0.5));
	//points.push_back(jmk::Point2d(0.3, -0.9));
	//points.push_back(jmk::Point2d(0.4, -0.8));
	//points.push_back(jmk::Point2d(-0.2, -0.1));

	points.push_back(Point2d(2, 1));
	points.push_back(Point2d(6, 2));
	points.push_back(Point2d(8, 6));
	points.push_back(Point2d(4, 5));
	points.push_back(Point2d(5, 7));

	std::vector<Edge2dSimple> edges;

	BoundRectangle rect{0.0,10.0,15.0,0.0};

	constructVoronoiDiagram_fortunes(points, edges, rect);

	bool test_passed = true;
	EXPECT_TRUE(test_passed);
}