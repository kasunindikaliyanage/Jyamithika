#pragma once
#pragma once

#define _USE_MATH_DEFINES

#include<math.h>

namespace jmk {
#define TOLERANCE 0.00001

	enum RELATIVE_POSITION {
		LEFT, RIGHT, BEYOND, BEHIND, BETWEEN, ORIGIN, DESTINATION
	};


	enum INTERSECTION_OPS {
		CROSSES,
		POSITIVE,
		NEGATIVE,
		COINCIDENT
	};

	static bool isEqualD(double x, double y)
	{
		return fabs(x - y) < TOLERANCE;
	}
}