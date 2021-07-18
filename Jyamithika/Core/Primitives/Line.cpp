#include "Line.h"

jmk::Vector3f jmk::Line::direction()
{
	return dir;
}

jmk::Point3d jmk::Line::point()
{
	return point_in_line;
}
