#pragma once

#include "../Primitives/Point.h"

namespace jmk {

	struct AABB {
		float x_min;
		float x_max;
		float y_min;
		float y_max;

		bool isInside(Point2d& point) {
			if (x_min <= point[X] && point[X] <= x_max
				&& y_min <= point[Y] && point[Y] <= y_max)
				return true;
			return false;
		}
	};
}