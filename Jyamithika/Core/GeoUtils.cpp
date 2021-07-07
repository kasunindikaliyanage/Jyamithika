#include "GeoUtils.h"

#include <math.h>

using namespace jmk;

float jmk::polarAngle( Point3d& _other, Point3d& _ref)
{
	if (_other[Z] == 0 && _ref[Z] == 0)
	{
		// Consider the given points as 2D ones which are in XY plane
		double _x = _other.data()[X] - _ref.data()[X];
		double _y = _other.data()[Y] - _ref.data()[Y];

		if ((isEqualD(_x, 0.0)) && (isEqualD(_y, 0.0)))
			return -1.0;
		if (isEqualD(_x, 0.0))
			return ((_y > 0.0) ? 90 : 270);

		double theta = atan(_y / _x);
		theta *= 360 / (2 * 3.141);

		if (_x > 0.0)
			return ((_y >= 0.0) ? theta : 360 + theta);
		else
			return (180 + theta);
	}
	else
	{
		// Points are in 3D space
		return 0.0f;
	}

}
