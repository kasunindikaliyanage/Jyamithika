#include "Convexhull.h"
#include "Core\GeoUtils.h"

#include <algorithm>

using namespace jmk;

void jmk::convexhull2DGiftwrapping(std::vector<Point3d>& _points, std::vector<Point3d>& _convex)
{
	if (_points.size() <= 3)
		return;

	// For giftwarpping algorithm we have to pick a point which should be in the convexhull.
	// This point could be top most, bot most, left most or right most point. I pick bot most

	// Get the bottom point
	Point3d bottom_point = _points[0];

	for (Point3d& point : _points)
	{
		if ((point[Y] < bottom_point[Y])
			|| (point[Y] == bottom_point[Y]) && (point[X] < bottom_point[X]) )
		{
			bottom_point = point;
		}
	}

	Point3d min_polar_point = _points[0];
	float current_polor_angle = 360.0;

	// Finding the second points in the convexhull by calculating poloar angle related to bottom point
	for (size_t i = 0; i < _points.size(); i++)
	{
		float polar_angle = polarAngle(_points[i], bottom_point);
		if (bottom_point != _points[i] && current_polor_angle > polar_angle)
		{
			current_polor_angle = polar_angle;
			min_polar_point = _points[i];
		}
	}
	
	// Add the first two points of the convexhull
	_convex.push_back(bottom_point);
	_convex.push_back(min_polar_point);
	
	// Make the second point of the current convexhull point list the referece to calculate next point.
	Point3d ref_point = min_polar_point;
	int index_before_last = 0;

	while (true)
	{
		current_polor_angle = 360.0;
		for (size_t i = 0; i < _points.size(); i++)
		{
			Vector3f vec1 = ref_point - _convex[index_before_last];
			Vector3f vec2 = _points[i] - ref_point;

			float between_angle = angle(vec1, vec2);

			if ( ref_point != _points[i] && current_polor_angle > between_angle)
			{
				current_polor_angle = between_angle;
				min_polar_point = _points[i];
			}
		}

		if (min_polar_point == bottom_point)
			break;

		index_before_last++;
		_convex.push_back(min_polar_point);
		ref_point = min_polar_point;
	}
}

void jmk::convexhull2DGrahams(std::vector<Point3d>& _points, std::vector<Point3d>& _convex)
{
	if (_points.size() <= 3)
		return;

	// Get the bottom point
	Point3d bottom_point = _points[0];

	for (Point3d& point : _points)
	{
		if ((point[Y] < bottom_point[Y])
			|| (point[Y] == bottom_point[Y]) && (point[X] < bottom_point[X]))
		{
			bottom_point = point;
		}
	}
}