#include "Base\Core.h"
#include "Intersection.h"


bool jmk::intersect(jmk::Line2d& l1, jmk::Line2d& l2, jmk::Point2d& pi)
{
	Vector2f l1p = l1.point();
	Vector2f l2p = l2.point();
	Vector2f l1d = l1.direction();
	Vector2f l2d = l2.direction();

	float a, b, c, d, e, f;

	a = l1d[X];
	b = -l2d[X];
	c = l2p[X] - l1p[X];
	d = l1d[Y];
	e = -l2d[Y];
	f = l2p[Y] - l1p[Y];

	Vector2f diff = l2p - l1p;
	auto prep_l2d = prependicluar(l2d);
	float dot_d0_prepd1 = dotProduct(l1d, prep_l2d);

	if (!isEqualD(dot_d0_prepd1, ZERO))
	{
		//// TODO need to check whether the intersection points is in correct side. Whether the lines 
		//// -are converge or not.
		//float invD0DotPerpD1 = (float)1 / dot_d0_prepd1;
		//float diffDotPerpD0 = dotProduct(diff, prep_l1d);
		//float diffDotPerpD1 = dotProduct(diff, prep_l2d);
		//float s0 = diffDotPerpD1 * invD0DotPerpD1;
		//float s1 = diffDotPerpD0 * invD0DotPerpD1;

		float denominator = a * e - b * d;
		float t_numerator = c * e - b * f;
		float s_numerator = a * f - c * a;

		float t = t_numerator / denominator;

		float x = l1p[X] + t * l1d[X];
		float y = l1p[Y] + t * l1d[Y];

		if ((x - l1p[X]) / l1d[X] < 0) return false;
		if ((y - l1p[Y]) / l1d[Y] < 0) return false;

		if ((x - l2p[X]) / l2d[X] < 0) return false;
		if ((y - l2p[Y]) / l2d[Y] < 0) return false;
	
		pi.assign(X, x);
		pi.assign(Y, y);
		return true;
	}
	else
	{
		//Lines are parallel
		return false;
	}
}