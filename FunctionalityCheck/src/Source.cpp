#include <iostream>

#include "Jyamithika\Core\Primitives\Point.h"
#include "Jyamithika\Trees\KDTree.h"


int main()
{
	std::list<jmk::Point2d> points;
	points.push_back(jmk::Point2d(1, 4));
	points.push_back(jmk::Point2d(2, 2));
	points.push_back(jmk::Point2d(3, 3));
	points.push_back(jmk::Point2d(4, 1));
	points.push_back(jmk::Point2d(5, 5));
	points.push_back(jmk::Point2d(6, 6));
	points.push_back(jmk::Point2d(7, 7));
	points.push_back(jmk::Point2d(8, 8));
	points.push_back(jmk::Point2d(9, 9));
	points.push_back(jmk::Point2d(10, 10));
	points.push_back(jmk::Point2d(11, 11));
	points.push_back(jmk::Point2d(12, 12));
	points.push_back(jmk::Point2d(13, 13));
	points.push_back(jmk::Point2d(14, 14));
	points.push_back(jmk::Point2d(15, 15));

	jmk::KDTree tree(points);

	std::cout << "Hello KDTree" << std::endl;
	return 0;
}