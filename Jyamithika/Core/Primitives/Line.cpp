#include "Line.h"

jmk::Vector3f jmk::Line::direction() const
{
	return dir;
}

jmk::Point3d jmk::Line::point() const
{
	return point_in_line;
}

jmk::Vector2f jmk::Line2d::direction() const
{
	return dir;
}

jmk::Point2d jmk::Line2d::point() const
{
	return point_in_line;
}
