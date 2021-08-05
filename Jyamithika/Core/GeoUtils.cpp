#include "GeoUtils.h"

#include <math.h>
#include <set>
#include <map>
#include <algorithm>
#include <list>

#include "Intersection.h"

using namespace jmk;

// This can be only used in 2d XY plane. 
// TODO  has to modify to consider the 3D plane as well
int jmk::relation3d(const Point3d& a, const Point3d& b, const Point3d& c)
{
	float area = areaTriangle2d(a, b, c);
	
	if (area > 0 && area < TOLERANCE)
		area = 0;

	if (area < 0 && area > TOLERANCE)
		area = 0;

	Point3d p1 = b - a;
	Point3d p2 = c - a;

	double p1x, p1y, p2x, p2y;

	p1x = p1[X];
	p1y = p1[Y];
	p2x = p2[X];
	p2y = p2[Y];

	if (area > 0.0)
		return LEFT;
	if (area < 0.0)
		return RIGHT;
	if ((p1x * p2x < 0.0) || (p1y * p2y < 0.0))
		return BEHIND;
	if (p1.magnitude() < p2.magnitude())
		return BEYOND;
	if (a == c)
		return ORIGIN;
	if (b == c)
		return DESTINATION;
	return BETWEEN;

	return 0;
}

int jmk::relation2d(const Point2d& a, const Point2d& b, const Point2d& c)
{
	float area = areaTriangle2d(a, b, c);

	if (area > 0 && area < TOLERANCE)
		area = 0;

	if (area < 0 && area > TOLERANCE)
		area = 0;

	Point2d p1 = b - a;
	Point2d p2 = c - a;

	double p1x, p1y, p2x, p2y;

	p1x = p1[X];
	p1y = p1[Y];
	p2x = p2[X];
	p2y = p2[Y];

	if (area > 0.0)
		return LEFT;
	if (area < 0.0)
		return RIGHT;
	if ((p1x * p2x < 0.0) || (p1y * p2y < 0.0))
		return BEHIND;
	if (p1.magnitude() < p2.magnitude())
		return BEYOND;
	if (a == c)
		return ORIGIN;
	if (b == c)
		return DESTINATION;
	return BETWEEN;

	return 0;
}

bool jmk::left(const Point3d& a, const Point3d& b, const Point3d& c)
{
	return relation3d(a,b,c) == RELATIVE_POSITION::LEFT;
}

bool jmk::left(const Point2d& a, const Point2d& b, const Point2d& c)
{
	return relation2d(a, b, c) == RELATIVE_POSITION::LEFT;
	//return relation3d(Point3d(a[X], a[Y], 0.0), Point3d(b[X], b[Y], 0.0), Point3d(c[X], c[Y], 0.0)) == RELATIVE_POSITION::LEFT;
}

bool jmk::right(const Point3d& a, const Point3d& b, const Point3d& c)
{
	return relation3d(a, b, c) == RELATIVE_POSITION::RIGHT;
}

bool jmk::leftOrBeyond(const Point2d& a, const Point2d& b, const Point2d& c)
{
	int position = relation2d(a, b, c);
	return (position == RELATIVE_POSITION::LEFT || position == RELATIVE_POSITION::BEYOND);
}

bool jmk::leftOrBeyond(const Point3d& a, const Point3d& b, const Point3d& c)
{
	int position = relation3d(a, b, c);
	return (position == RELATIVE_POSITION::LEFT || position == RELATIVE_POSITION::BEYOND);
}

bool jmk::leftOrBetween(const Point3d& a, const Point3d& b, const Point3d& c)
{
	int position = relation3d(a, b, c);
	return (position == RELATIVE_POSITION::LEFT || position == RELATIVE_POSITION::BETWEEN);
}

static bool incone(const Vertex2dSimple* v1, const Vertex2dSimple* v2)
{
	if (jmk::leftOrBeyond(v1->point, v1->next->point, v1->prev->point)) {
		// v1 is vonvx vertex
		return jmk::left(v1->point, v2->point, v1->prev->point)
			&& jmk::left(v2->point, v1->point, v1->next->point);
	}

	// v1 is relex vertex
	return !(jmk::leftOrBeyond(v1->point, v2->next->point, v1->next->point)
		&& jmk::leftOrBeyond(v2->point, v1->next->point, v1->prev->point));
}

bool jmk::isDiagonal( const Vertex2dSimple* v1, const Vertex2dSimple* v2, Polygon2dSimple* poly)
{
	bool prospect = true;
	std::vector<Vertex2dSimple*> vertices;
	if(poly)
		vertices = poly->getVertcies();
	else {
		auto vertex_ptr = v1->next;
		vertices.push_back((Vertex2dSimple*)v1);
		while (vertex_ptr != v1) {
			vertices.push_back(vertex_ptr);
			vertex_ptr = vertex_ptr->next;
		}
	}

	Vertex2dSimple *current, *next;
	current = vertices[0];
	do{
		next = current->next;
		if (current != v1 && next != v1 && current != v2 && next != v2
			&& jmk::intersect(v1->point, v2->point, current->point, next->point)) {
			prospect = false;
			break;
		}
		current = next;
	} while (current != vertices[0]);

	return prospect && incone(v1, v2) && incone(v2, v1);
}

float jmk::polarAngle( const Point2d& _other, const Point2d& _ref)
{
	// Consider the given points as 2D ones which are in XY plane
	float _x = _other[X] - _ref[X];
	float _y = _other[Y] - _ref[Y];

	if ((isEqualD(_x, 0.0)) && (isEqualD(_y, 0.0)))
		return -1.0;
	if (isEqualD(_x, 0.0))
		return ((_y > 0.0) ? 90 : 270);

	double theta = atan(_y / _x);
	theta *= 360 / (2 * M_PI);

	if (_x > 0.0)
		return ((_y >= 0.0) ? theta : 360 + theta);
	else
		return (180 + theta);
}

double jmk::areaTriangle2d(const Point3d& a, const Point3d& b, const Point3d& c)
{
	return 0.5 * ((b[X] - a[X]) * (c[Y] - a[Y]) - (c[X] - a[X]) * (b[Y] - a[Y]));
}

double jmk::areaTriangle2d(const Point2d& a, const Point2d& b, const Point2d& c)
{
	return 0.5 * ((b[X] - a[X]) * (c[Y] - a[Y]) - (c[X] - a[X]) * (b[Y] - a[Y]));
}

double jmk::areaTriangle3d(const Point3d& a, const Point3d& b, const Point3d& c)
{
	float x_, y_, z_;

	Vector3f AB = b - a;
	Vector3f AC = c - a;

	x_ = AB[Y] * AC[Z] - AB[Z] * AC[Y];
	y_ = AB[X] * AC[Z] - AB[Z] * AC[X];
	z_ = AB[X] * AC[Y] - AB[Y] * AC[X];

	float sum_of_powers = pow(x_,2.0) + pow(y_, 2.0) + pow(z_, 2.0);
	float root = sqrtf(sum_of_powers);
	return root / 2;
}

int jmk::orientation(const Face& _f, const Vector3f& _p)
{
	// If the plane of the face is prependicular to XY plane
	// In such case We cannot simply consider as z=0; But we can set either x= 0 or y = 0.
	std::vector<Point3d> point_vec;

	for (size_t i = 0; i < _f.vertices.size(); i++)
	{
		point_vec.push_back(*_f.vertices[i]->point);
	}

	Planef plane(point_vec[0], point_vec[1], point_vec[2]);

	/*
		Taking the dot product of the normal with a vector from the given view point, V, 
		to one of the vertices will give you a value whose sign indicates which way the vertices 
		appear to wind when viewed from V:
	*/

	Vector3f PA = point_vec[0] - _p;
	float winding_constant = dotProduct(plane.getNormal(), PA);

	// If winding constant is negative, it means from the _p point of view, points in the plane is counter clockwise
	if (winding_constant < ZERO)
		return CCW;

	return CW;
}

float jmk::volumeSigned(const Face& _f, const Point3d& _p)
{
	double ax, ay, az, bx, by, bz, cx, cy, cz;

	Point3d p1, p2, p3;
	p1 = *_f.vertices[0]->point;
	p2 = *_f.vertices[1]->point;
	p3 = *_f.vertices[2]->point;

	ax = p1[X] - _p[X];
	ay = p1[Y] - _p[Y];
	az = p1[Z] - _p[Z];
	bx = p2[X] - _p[X];
	by = p2[Y] - _p[Y];
	bz = p2[Z] - _p[Z];
	cx = p3[X] - _p[X];
	cy = p3[Y] - _p[Y];
	cz = p3[Z] - _p[Z];

	double vol = ax * (by * cz - cy * bz) + ay * (bz * cx - cz * bx) + az * (bx * cy - by * cx);
	return vol;

	//	// If the plane of the face is prependicular to XY plane
	//// In such case We cannot simply consider as z=0; But we can set either x= 0 or y = 0.
	//std::vector<Point3d> point_vec;

	//for (size_t i = 0; i < _f.vertices.size(); i++)
	//{
	//	point_vec.push_back(*_f.vertices[i]->point);
	//}

	//Vector3f ref_vec1(1.0, 1.0, 0.0); // This point is in xy plane Directional vector AO
	//Vector3f ref_vec2(-1.0, 1.0, 0.0); // This point is in xy plane Directional vector BO

	//Planef plane(point_vec[0], point_vec[1], point_vec[2]);

	///*
	//	Taking the dot product of the normal with a vector from the given view point, V,
	//	to one of the vertices will give you a value whose sign indicates which way the vertices
	//	appear to wind when viewed from V:
	//*/

	//Vector3f PA = point_vec[0] - _p;
	//float winding_constant = dotProduct(plane.getNormal(), PA);
	//return winding_constant;
}

float jmk::angle(const Vector2f& _v1, const Vector2f& _v2)
{
	float dot = dotProduct(_v1, _v2);
	float v1_mag = _v1.magnitude();
	float v2_mag = _v2.magnitude();
	auto deno = v1_mag * v2_mag;
	if (isEqualDL(dot, deno))
		return 0;

	return acos(dot / (v1_mag * v2_mag));
}

bool jmk::isInside(Point3d& _point, std::vector<Point3d>& _points)
{
	return true;
}

bool jmk::isInside(Point3d& _point, std::vector<Face>& _faces)
{
	// Before the check we have to confirm the orientaion of the points. (CW or CCW)



	return true;
}

int jmk::getClosestPointIndex(Point3d& _point, std::vector<Point3d>& _points)
{
	return 0;
}

bool jmk::collinear(const Point3d& a, const Point3d& b, const Point3d& c)
{
	// No need to calculat the area. Just compare each coeficient vector to zero vec;
	float x_, y_, z_;

	Vector3f AB = b - a;
	Vector3f AC = c - a;

	x_ = AB[Y] * AC[Z] - AB[Z] * AC[Y];
	y_ = AB[X] * AC[Z] - AB[Z] * AC[X];
	z_ = AB[X] * AC[Y] - AB[Y] * AC[X];
	
	return isEqualD(x_, ZERO) && isEqualD(y_, ZERO) && isEqualD(z_, ZERO);
}

bool jmk::coplaner(const Point3d& a, const Point3d& b, const Point3d& c, const Point3d& d)
{
	Vector3f AB = b - a;
	Vector3f AC = c - a;
	Vector3f AD = d - a;
	return coplaner(AB, AC, AD);
}

bool jmk::coplaner(const Vector3f& _v1, const Vector3f& _v2, const Vector3f& _v3)
{
	float value = scalerTripleProduct(_v1, _v2, _v3);
	return isEqualD(value, ZERO);
}

bool jmk::segmentIsLeft(const Segment2d& base, const Segment2d& compare, const Point2d& _point)
{
	return base.get_x(_point[Y]) < compare.get_x(_point[Y]);
}



