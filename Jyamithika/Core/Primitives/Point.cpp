
#include "Point.h"

bool jmk::sort2DLRTB(const Point2d& a, const Point2d& b)
{
	if ((a[X] < b[X])
		|| (a[X] == b[X]) && (a[Y] < b[Y]))
	{
		return true;
	}
	return false;
}

bool jmk::sort3DTBLR(const Point3d& a, const Point3d& b)
{
	if ((a[Y] > b[Y])
		|| (a[Y] == b[Y]) && (a[X] < b[X]))
	{
		return true;
	}
	return false;
}

bool jmk::sort2DTBLR(const Point2d& a, const Point2d& b)
{
	if ((a[Y] > b[Y])
		|| (a[Y] == b[Y]) && (a[X] < b[X]))
	{
		return true;
	}
	return false;
}

bool jmk::sort2DbyX(const Point2d& a, const Point2d& b)
{
	if (a[X] < b[X])
		return true;
	return false;
}

bool jmk::sort2DbyY(const Point2d& a, const Point2d& b)
{
	if (a[Y] < b[Y])
		return true;
	return false;
}
