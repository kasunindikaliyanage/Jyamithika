#include "Convexhull.h"
#include "Core\GeoUtils.h"

#include <algorithm>

using namespace jmk;

void jmk::convexhull2dGiftwrapping(std::vector<Point3d>& _points, Polygon& _polygon)
{
	if (_points.size() <= 3)
		return;

	// Get the TOP and BOTTOM points
	Point3d bottom_point = _points[0];
	Point3d top_point = _points[0];

	for (Point3d& point : _points)
	{
		if ((point[Y] < bottom_point[Y])
			|| (point[Y] == bottom_point[Y]) && (point[X] < bottom_point[X]) )
		{
			bottom_point = point;
		}

		if ((point[Y] > top_point[Y])
			|| (point[Y] == top_point[Y]) && (point[X] > top_point[X]))
		{
			top_point = point;
		}
	}

	Point3d min_polar_point = _points[0];
	Point3d ref_point = bottom_point;
	float current_polor_angle = 360.0;

	for (size_t i = 0; i < _points.size(); i++)
	{
		if (bottom_point != _points[i])
		{
			float polar_angle = polarAngle(_points[i], ref_point);
			if (current_polor_angle > polar_angle)
			{
				current_polor_angle = polar_angle;
				min_polar_point = _points[i];
			}
		}
	}

	while (min_polar_point != bottom_point)
	{
		for (size_t i = 0; i < _points.size(); i++)
		{
			float polar_angle = polarAngle(_points[i], ref_point);
			if (current_polor_angle > polar_angle)
			{
				current_polor_angle = polar_angle;
				min_polar_point = _points[i];
			}
		}
	}
}