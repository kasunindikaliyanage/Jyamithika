#include "Intersection.h"


bool jmk::intersect(jmk::Line2d& l1, jmk::Line2d& l2, jmk::Point2d& pi)
{
	Vector2f l1p = l1.point();
	Vector2f l2p = l2.point();
	Vector2f l1d = l1.direction();
	Vector2f l2d = l2.direction();

	Vector2f E = l1p - l2p;
	float kross = l1d[X] * l2d[Y] - l1d[Y] * l2d[X];
	float sqrKross = kross * kross;
	float sqrLen1 = l1d[X] * l1d[X] + l1d[Y] * l1d[Y];
	float sqrLen2 = l2d[X] * l2d[X] + l2d[Y] * l2d[Y];
	if (sqrKross > 0.0001 * sqrLen1 * sqrLen2) {
		// lines are not parallel
		float s = (E[X] * l2d[Y] - E[Y] * l2d[X]) / kross;
		Vector2f idir(s * l1d[X], s * l1d[Y]);
		Vector2f inter(l1p + idir);
		pi.assign(X, inter[X]);
		pi.assign(Y, inter[Y]);
		return true;
	}
	return false;
}