#pragma once
#pragma once

#include<math.h>

namespace jmk {
#define TOLERANCE 0.00001

	static bool isEqualD(double x, double y)
	{
		return fabs(x - y) < TOLERANCE;
	}
}