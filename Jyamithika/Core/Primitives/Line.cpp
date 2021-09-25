#include "Line.h"

void jmk::Line::setDirection(Vector3f& _dir)
{
	dir.assign(X, _dir[X]);
	dir.assign(Y, _dir[Y]);
	dir.assign(Z, _dir[Z]);
}

void jmk::Line::setPoint(Point3d& _point)
{
	point_in_line.assign(X, _point[X]);
	point_in_line.assign(Y, _point[Y]);
	point_in_line.assign(Z, _point[Z]);
}

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

jmk::Vector2f jmk::Line2d::normal() const
{
	return normal_vec;
}
