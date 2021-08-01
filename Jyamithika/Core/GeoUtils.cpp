#include "GeoUtils.h"

#include <math.h>
#include <set>
#include <map>
#include <algorithm>
#include <list>

using namespace jmk;


enum class VERTEX_CATEGORY {
	START,
	END,
	REGULAR,
	SPLIT,
	MERGE,
	INVALID
};

// static functions

VERTEX_CATEGORY categorize_vertex(Vertex2d& v)
{
	Vertex2d* v_prev = v.prev;
	Vertex2d* v_next = v.next;

	// Need vertex beside the given one to get the dicission. Return INVALID if one is missing
	if (!v_prev || !v_next)
		return VERTEX_CATEGORY::INVALID;

	Point2d p_prev = v_prev->point;
	Point2d p = v.point;
	Point2d p_next = v_next->point;

	float p_prev_y = p_prev[Y];
	float p_y = p_prev[Y];
	float p_next_y = p_next[Y];

	bool is_left = left(p_prev, p, p_next);

	if (p_y > p_prev_y && p_y > p_next_y)
	{
		if (is_left)
			return VERTEX_CATEGORY::START;
		else
			return VERTEX_CATEGORY::SPLIT;
	}
	else if (p_y < p_prev_y && p_y < p_next_y)
	{
		if (is_left)
			return VERTEX_CATEGORY::END;
		else
			return VERTEX_CATEGORY::MERGE;
	}
	else
	{
		return VERTEX_CATEGORY::REGULAR;
	}
}

// Custome comparator to be used in sweep line status data structure.
struct CustomeComparator {
	Point2d* sweep_line_point;

	CustomeComparator(Point2d* _point)
	{
		sweep_line_point = _point;
	}

	//Return true the segment _a is left to the segment _b in the sweep line point
	bool operator()(Segment2d* _a, Segment2d* _b) const
	{
		if (_a == _b)
			return false;
		return segmentIsLeft(*_a, *_b, *sweep_line_point);
	}
};

struct Sweep_line {
	std::list<Edge2d> edge_list;
	Point2d* point;

	Sweep_line(Point2d* _point) :point(_point) {}
};

struct vertex_ordering_from_id {
	bool operator() (const Vertex2d& lhs, const Vertex2d& rhs) const
	{
		return true;
	}
};

void get_vertex_edge_map(Polygon2d &poly,std::map<Vertex2d, Edge2d, vertex_ordering_from_id>& v_e_map)
{
	std::vector<Vertex2d> vertices = poly.getVertcies();
	if (!vertices.size() > 3)
		return;

	Vertex2d* root = &vertices[0];
	Vertex2d* current = &vertices[1];

	if (root != current)
	{
		v_e_map.insert(std::pair<Vertex2d, Edge2d>(*root, Edge2d(root->point, current->point)));
	}
	//Create the edges from vertices
	while (current != root)
	{
		v_e_map.insert(std::pair<Vertex2d, Edge2d>(*current, Edge2d(current->point, current->next->point)));
		current = current->next;
	}
}

static void handle_start_vertices(Vertex* start, std::set<Segment2d*, CustomeComparator>& T, std::vector<Edge2d*>& diagonals)
{
}

static void handle_end_vertices(Vertex* start, std::set<Segment*, CustomeComparator>& T, std::vector<Edge2d*>& diagonals)
{

}

static void handle_split_vertices(Vertex* start, std::set<Segment*, CustomeComparator>& T, std::vector<Edge2d*>& diagonals)
{

}

static void handle_merge_vertices(Vertex* start, std::set<Segment*, CustomeComparator>& T, std::vector<Edge2d*>& diagonals)
{

}

static void handle_regular_vertices(Vertex* start, std::set<Segment*, CustomeComparator>& T, std::vector<Edge2d*>& diagonals)
{

}


// This can be only used in 2d XY plane. 
// TODO  has to modify to consider the 3D plane as well
int jmk::pointRelativePos(const Point3d& a, const Point3d& b, const Point3d& c)
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

bool jmk::left(const Point3d& a, const Point3d& b, const Point3d& c)
{
	return pointRelativePos(a,b,c) == RELATIVE_POSITION::LEFT;
}

bool jmk::left(const Point2d& a, const Point2d& b, const Point2d& c)
{
	return pointRelativePos(Point3d(a[X], a[Y], 0.0), Point3d(b[X], b[Y], 0.0), Point3d(c[X], c[Y], 0.0)) == RELATIVE_POSITION::LEFT;
}

bool jmk::right(const Point3d& a, const Point3d& b, const Point3d& c)
{
	return pointRelativePos(a, b, c) == RELATIVE_POSITION::RIGHT;
}

bool jmk::leftOrBeyond(const Point3d& a, const Point3d& b, const Point3d& c)
{
	int position = pointRelativePos(a, b, c);
	return (position == RELATIVE_POSITION::LEFT || position == RELATIVE_POSITION::BEYOND);
}

bool jmk::leftOrBetween(const Point3d& a, const Point3d& b, const Point3d& c)
{
	int position = pointRelativePos(a, b, c);
	return (position == RELATIVE_POSITION::LEFT || position == RELATIVE_POSITION::BETWEEN);
}

float jmk::polarAngle( const Point3d& _other, const Point3d& _ref)
{
	if (_other[Z] == 0 && _ref[Z] == 0)
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
	else
	{
		// Points are in 3D space
		return 0.0f;
	}

}

double jmk::areaTriangle2d(const Point3d& a, const Point3d& b, const Point3d& c)
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

float jmk::angle(const Vector3f& _v1, const Vector3f& _v2)
{
	float dot = dotProduct(_v1, _v2);
	float v1_mag = _v1.magnitude();
	float v2_mag = _v2.magnitude();

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

void jmk::get_monotone_polygons(Polygon2d* poly, std::vector<Polygon2d>& mono_polies)
{
	std::vector<Vertex2d> vertex_list;
	std::vector<Edge2d> diagonal_list;

	vertex_list = poly->getVertcies();

	//Sort the vertex_list from vertex with highest y to lowest y
	std::sort(vertex_list.begin(), vertex_list.end(), [](auto x, auto y) 
	{
		Point2d a = x.point;
		Point2d b = y.point;
		if ((a[Y] > b[Y])
			|| (a[Y] == b[Y]) && (a[X] < b[X]))
		{
			return true;
		}
		return false;
	});

	Vertex2d top_vertex = *vertex_list.begin();

	std::map<Vertex2d, Edge2d, vertex_ordering_from_id> v_e_map;
	get_vertex_edge_map(*poly,v_e_map);

	// Configuring T. We can update the sweep point to move the sweep line downwards and rearrange the edges
	Point2d* sweep_point = new Point2d(top_vertex.point[X], top_vertex.point[Y]);
	Sweep_line T(sweep_point);

	// TODO for now just store the sweep line a std::list and in each split or merge vertex we recalculate the order 
	// -depending on the point and find out the left edge to the given vertex

	Edge2d temp_edge;
	Vertex* temp_vertex;

	std::map<Vertex, Edge2d>::iterator itr;
	for (auto vertex_ptr : vertex_list)
	{
		VERTEX_CATEGORY v_cat = categorize_vertex(vertex_ptr);

		////temp_edge = new Edge(vertex_ptr->get_point(), vertex_ptr->get_next()->get_point());
		//temp_edge = v_e_map.find(vertex_ptr)->second;

		//if (v_cat == VERTEX_CATEGORY::START)
		//{
		//	temp_edge.set_helper(vertex_ptr);
		//	T.push(temp_edge);
		//}
		//else if (v_cat == VERTEX_CATEGORY::END)
		//{
		//	//Need to find the helper of the E(i-1)
		//	itr = v_e_map.find(*vertex_ptr);
		//	if (itr != v_e_map.begin())
		//		itr = --itr;
		//	else
		//		itr = --v_e_map.end();
		//	//Get helper vertex of the E(i-1)
		//	temp_vertex = itr->second.get_helper();

		//	if (VERTEX_CATEGORY::MERGE == categorize_vertex(temp_vertex))
		//	{
		//		diagonal_list.push_back(Edge(vertex_ptr, temp_vertex));
		//	}

		//	T.erase(itr->second);
		//}
		//else if (v_cat == VERTEX_CATEGORY::SPLIT)
		//{
		//	sweep_point->assign(vertex_ptr->get_point()->get_x(), vertex_ptr->get_point()->get_x());
		//	T.rearrange();
		//	// need a way to retrieve the left edge to the vertex from the T 
		//	bool left_edge_found = T.get_edge_to_left(temp_edge);

		//	if (left_edge_found)
		//	{
		//		diagonal_list.push_back(Edge(vertex_ptr, temp_edge.get_helper()));
		//		//temp_edge
		//	}
		//}
		//else if (v_cat == VERTEX_CATEGORY::MERGE)
		//{
		//}
		//else
		//{
		//	// Regular vertex handling
		//}
	}
}

