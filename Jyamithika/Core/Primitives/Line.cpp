#include "Line.h"

jmk::Vector3f jmk::Line::direction()
{
	return dir;
}

jmk::Point3d jmk::Line::point()
{
	return point_in_line;
}

jmk::Vector2f jmk::Line2d::direction()
{
	return dir;
}

jmk::Point2d jmk::Line2d::point()
{
	return point_in_line;
}
