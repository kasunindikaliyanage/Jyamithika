#include "Convexhull.h"
#include "Core\GeoUtils.h"
#include "Core\Distance.h"
#include "Core\Inclusion.h"

#include <algorithm>
#include <Iterator>
#include <list>

using namespace jmk;

void jmk::convexhull2DGiftwrapping(std::vector<Point2d>& _points, std::vector<Point2d>& _convex)
{
	if (_points.size() <= 3)
		return;

	// For giftwarpping algorithm we have to pick a point which should be in the convexhull.
	// This point could be top most, bot most, left most or right most point. I pick bot most

	// Get the bottom point
	Point2d bottom_point = _points[0];

	for (Point2d& point : _points){
		if ((point[Y] < bottom_point[Y])
			|| (point[Y] == bottom_point[Y]) && (point[X] < bottom_point[X]) ){
			bottom_point = point;
		}
	}

	Point2d min_polar_point = _points[0];
	float current_polor_angle = 360.0;

	// Finding the second points in the convexhull by calculating poloar angle related to bottom point
	for (size_t i = 0; i < _points.size(); i++){
		float polar_angle = polarAngle(_points[i], bottom_point);
		if (bottom_point != _points[i] && current_polor_angle > polar_angle){
			current_polor_angle = polar_angle;
			min_polar_point = _points[i];
		}
	}
	
	// Add the first two points of the convexhull
	_convex.push_back(bottom_point);
	_convex.push_back(min_polar_point);
	
	// Make the second point of the current convexhull point list the referece to calculate next point.
	Point2d ref_point = min_polar_point;
	int index_before_last = 0;

	while (true){
		current_polor_angle = 360.0;
		for (size_t i = 0; i < _points.size(); i++){
			Vector2f vec1 = ref_point - _convex[index_before_last]; // vector for line from last two vert in the convexhull
			Vector2f vec2 = _points[i] - ref_point;

			float between_angle = angle(vec1, vec2);
			if ( ref_point != _points[i] && current_polor_angle > between_angle){
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



void jmk::convexhull2DModifiedGrahams(std::vector<Point2d>& _points, 
	std::vector<Point2d>& _convex)
{
	if (_points.size() <= 3)
		return;

	//Sort the points left to right order
	std::sort(_points.begin(), _points.end());

	std::vector<Point2d> l_upper;
	std::vector<Point2d> l_lower;

	// Append left most point and next one l_upper.
	l_upper.push_back(*_points.begin());
	l_upper.push_back(*(std::next(_points.begin())));

	int index = 0;
	for (size_t i = 2; i < _points.size(); i++)
	{
		index = l_upper.size();
		const auto& next_point = _points[i];
		while (l_upper.size() > 1 && left(l_upper[index - 2], l_upper[index - 1], next_point))
		{
			l_upper.pop_back();
			index = l_upper.size();
		}

		l_upper.push_back(next_point);
	}

	//Reverse the points right to left order to construct l_lower
	std::reverse(_points.begin(), _points.end());

	// Append Right most point and next one l_lower.
	l_lower.push_back(*_points.begin());
	l_lower.push_back(*(std::next(_points.begin())));

	for (size_t i = 2; i < _points.size(); i++)
	{
		index = l_lower.size();
		const auto& next_point = _points[i];

		while (l_lower.size() > 1 && left(l_lower[index - 2], l_lower[index - 1], next_point))
		{
			l_lower.pop_back();
			index = l_lower.size();
		}

		l_lower.push_back(next_point);
	}

	// Left-most and Right-most points are repeating. So removed those from one of half hulls
	l_upper.pop_back();
	l_lower.pop_back();

	_convex.insert(_convex.end(), l_upper.begin(), l_upper.end());
	_convex.insert(_convex.end(), l_lower.begin(), l_lower.end());
}

void jmk::convexhull2DIncremental(std::vector<Point3d>& _points, std::vector<Point3d>& _convex)
{
	//Sort the points left to right order
	std::sort(_points.begin(), _points.end(), [](const Point3d& a, const Point3d& b) {
		if ((a[X] < b[X])
			|| (a[X] == b[X]) && (a[Y] < b[Y]))
		{
			return true;
		}
		return false;
	});

	_convex.push_back(_points[0]);
	_convex.push_back(_points[1]);
	_convex.push_back(_points[2]);

	int points_size = _points.size();

	for (size_t i = 3; i < _points.size(); i++)
	{
		// If the points is inside the current hull, we can skip it
		if(isInside(_points[i], _convex))
			continue;

		// Get the index of closetest point in current hull 
		int index = getClosestPointIndex(_points[i], _convex);
		Point3d line_point = _convex[index];
		Point3d current_point;
		int uindex = index + 1;
		while (true)
		{
			current_point = _convex[uindex % points_size];
			
			if (!right(_points[i], line_point, current_point))
				break;

			line_point = current_point;
			uindex += 1;
		}
		
		line_point = _convex[index];
		int lindex = index - 1;
		while (true)
		{
			if (lindex < 0)
				lindex = points_size - 1;

			current_point = _convex[lindex];

			if (!left(_points[i], line_point, current_point))
				break;

			line_point = current_point;
			lindex -= 1;
		}

		std::vector<Point3d> new_convex;

		if (lindex < uindex)
		{
			new_convex.insert(new_convex.begin(), _convex.begin(), _convex.begin() + lindex);
			new_convex.push_back(_convex[lindex]);
			new_convex.insert(new_convex.end(), _convex.begin()+uindex, _convex.end());
		}
		else
		{
			new_convex.insert(new_convex.begin(), _convex.begin() + uindex, _convex.begin()+lindex);
			new_convex.push_back(_convex[lindex]);
		}

		_convex = new_convex;
	}
}

template<typename Iterator>
void getHull(Iterator first, Iterator last, Polygon& _results)
{
	unsigned const length = std::distance(first, last);

	if (length == 1)
	{
		_results.Insert(*first);
	}
	else
	{
		Iterator const mid_point = first + length / 2;
		
		Polygon left_poly;
		Polygon right_poly;

		getHull(first, mid_point, left_poly);
		getHull(mid_point,last, right_poly);

		merge(left_poly, right_poly, _results);
	}
}

void jmk::convexhull2DDivideAndConquer(std::vector<Point3d>& _points, Polygon& _results)
{
	//Sort the points left to right order
	std::sort(_points.begin(), _points.end(), [](const Point3d& a, const Point3d& b) {
		if ((a[X] < b[X])
			|| (a[X] == b[X]) && (a[Y] < b[Y]))
		{
			return true;
		}
		return false;
	});

	getHull(_points.begin(), _points.end(), _results);
}

static void find_hull(std::vector<Point3d>& _points, std::vector<Point3d>& _convex, Point3d& _l, Point3d& _r)
{
	if (!_points.size())
		return;

	if (_points.size() == 1) 
	{
		_convex.push_back(_points[0]);
		return;
	}

	Point3d maxd_point = _points[0];
	float max_d = 0;

	for (Point3d& pnt : _points)
	{
		float this_distance = distance(_l, _r, pnt);
		if (this_distance > max_d)
		{
			max_d = this_distance;
			maxd_point = pnt;
		}
	}

	_convex.push_back(maxd_point);

	std::vector<Point3d> s1;
	std::vector<Point3d> s2;

	for (Point3d& pnt : _points)
	{
		if (leftOrBetween(_l, maxd_point, pnt))
			s1.push_back(pnt);
		else if (left(maxd_point, _r, pnt))
			s2.push_back(pnt);
	}

	find_hull(s1, _convex, _l, _r);
	find_hull(s2, _convex, _r, _l);
}

void jmk::convexhull2DQuickhull(std::vector<Point3d>& _points, std::vector<Point3d>& _convex)
{
	if (_points.size() <= 3)
		return;

	// Step 1 : Get the two extreme points Left top and right bot
	Point3d left_top = _points[0];
	Point3d right_bot = _points[0];

	for (Point3d& point : _points)
	{
		if ((point[X] < left_top[X])
			|| (point[X] == left_top[X]) && (point[Y] > left_top[X]))
		{
			left_top = point;
		}

		if ((point[X] > right_bot[X])
			|| (point[X] == right_bot[X]) && (point[Y] < right_bot[X]))
		{
			right_bot = point;
		}
	}

	_convex.push_back(left_top);
	_convex.push_back(right_bot);

	std::vector<Point3d> s1;
	std::vector<Point3d> s2;

	for (Point3d& point : _points)
	{
		if (leftOrBetween(left_top, right_bot, point))
			s1.push_back(point);
		else if(right(left_top, right_bot, point))
			s2.push_back(point);
	}

	find_hull(s1, _convex, left_top, right_bot);
	find_hull(s2, _convex, right_bot,left_top);
}

static bool incident_face(Face* _face, Edge3d* _edge)
{
	auto r1 = std::find(_face->vertices.begin(), _face->vertices.end(), _edge->vertices[0]);
	auto r2 = std::find(_face->vertices.begin(), _face->vertices.end(), _edge->vertices[1]);
	if (r1 != std::end(_face->vertices) && r1 != std::end(_face->vertices))
		return true;
	return false;
}

static void adjust_normal(Face* _face, Point3d& _ref_point)
{
	// ref point is inside one. If it sees the orientation as counter clockwise, we need to adjust the face normal direction.
	// If the this is opposite what we need. 
	int order = FaceVisibility(*_face, _ref_point);
	if (order < 0 ){
		_face->normal_switch_needed = true;
	}
}

static void construct_initial_polyhedron(std::vector<Vertex3d*>& _points, int i,std::vector<Face*> &faces, std::vector<Edge3d*> &edges, 
	Point3d& ref_point)
{
	// Create the initial tetrahedron
	faces.push_back(new Face(_points[i + 0], _points[i + 1], _points[i + 2]));
	faces.push_back(new Face(_points[i + 0], _points[i + 1], _points[i + 3]));
	faces.push_back(new Face(_points[i + 1], _points[i + 2], _points[i + 3]));
	faces.push_back(new Face(_points[i + 2], _points[i + 0], _points[i + 3]));

	for (size_t i = 0; i < faces.size(); i++)
	{
		adjust_normal(faces[i], ref_point);
	}

	edges.push_back(new Edge3d(_points[i + 0], _points[i + 1]));
	edges.push_back(new Edge3d(_points[i + 1], _points[i + 2]));
	edges.push_back(new Edge3d(_points[i + 2], _points[i + 0]));
	edges.push_back(new Edge3d(_points[i + 0], _points[i + 3]));
	edges.push_back(new Edge3d(_points[i + 1], _points[i + 3]));
	edges.push_back(new Edge3d(_points[i + 2], _points[i + 3]));

	//Set the boundary edges for faces
	faces[0]->addEdge(edges[0]);
	faces[0]->addEdge(edges[1]);
	faces[0]->addEdge(edges[2]);

	faces[1]->addEdge(edges[0]);
	faces[1]->addEdge(edges[3]);
	faces[1]->addEdge(edges[4]);

	faces[2]->addEdge(edges[1]);
	faces[2]->addEdge(edges[4]);
	faces[2]->addEdge(edges[5]);

	faces[3]->addEdge(edges[2]);
	faces[3]->addEdge(edges[5]);
	faces[3]->addEdge(edges[3]);

	// set the incident faces for edges
	edges[0]->faces[0] = faces[0];
	edges[0]->faces[1] = faces[1];

	edges[1]->faces[0] = faces[0];
	edges[1]->faces[1] = faces[2];

	edges[2]->faces[0] = faces[0];
	edges[2]->faces[1] = faces[3];

	edges[3]->faces[0] = faces[1];
	edges[3]->faces[1] = faces[3];

	edges[4]->faces[0] = faces[2];
	edges[4]->faces[1] = faces[1];

	edges[5]->faces[0] = faces[3];
	edges[5]->faces[1] = faces[2];
}

void jmk::convexhull3D(std::vector<Point3d>& _points, std::vector<Face*>& faces)
{
	// Step 1 : Pick 4 points that do not lie in same plane. If we cannot find such points
	//			- then all the points as in one plane and we can use 2d convexhull algo to find the hull.
	//				1. Pick two points.(P1, P2)
	//				2. Pick thired point that do not in P1, P2 line. P3
	//				3. Pick fourth point that do not lie in P1, P2, P3 plane.

	std::vector<Vertex3d*> vertices;
	for (size_t i = 0; i < _points.size(); i++)
	{
		vertices.push_back(new Vertex3d(&_points[i]));
	}

	std::vector<Edge3d*> edges;

	size_t i = 0 , j =0 ;
	bool found_noncoplaner = false;
	for (  i = 0; i < _points.size() -3 ; i++ )
	{
		if (!coplaner(_points[i], _points[i + 1], _points[i + 2], _points[i + 3]))
		{
			found_noncoplaner = true;
			break;
		}
	}

	if (!found_noncoplaner)
	{
		std::cout << "All the points are coplaner" << std::endl;
		return;
	}

	// We need to find a point inside the 
	float x_mean = (_points[i][X] + _points[i + 1][X] + _points[i + 2][X] + _points[i + 3][X]) / 4;
	float y_mean = (_points[i][Y] + _points[i + 1][Y] + _points[i + 2][Y] + _points[i + 3][Y]) / 4;
	float z_mean = (_points[i][Z] + _points[i + 1][Z] + _points[i + 2][Z] + _points[i + 3][Z]) / 4;
	float x_p = x_mean ;
	float y_p = y_mean ;
	float z_p = z_mean ;

	Point3d point_ref(x_p, y_p, z_p);
	construct_initial_polyhedron(vertices, i, faces, edges, point_ref);
	
	//Points used to construct the p
	vertices[i]->processed = true;
	vertices[i+1]->processed = true;
	vertices[i+2]->processed = true;
	vertices[i+3]->processed = true;

	// Step 2 : Add next point Pr to the current convexhull
	//				1. Pr can be inside the current hull. Then there's nothing to be done.
	//				2. Pr lies outside the convexhull. In this case we need to compute new hull.

	for (size_t i = 0; i < vertices.size(); i++)
	{
		if (vertices[i]->processed)
			continue;

		std::vector<Face*> visible_faces;
		std::vector<Edge3d*> border_edges;
		std::vector<Edge3d*> tobe_deleted_edges;

		// Point has not yet processed and it is outside the current hull.
		for (size_t j = 0; j < faces.size(); j++)
		{
			float volum = FaceVisibility(*faces[j], *vertices[i]->point);

			//if (order == CCW && volum < 0 || order == CW && volum > 0)
			if ((!faces[j]->normal_switch_needed && volum < 0)
				|| (faces[j]->normal_switch_needed && volum > 0))
			{
				faces[j]->visible = true;
				visible_faces.push_back(faces[j]);
			}
		}

		if (!visible_faces.size())
			continue;	// Point is inside

		for (size_t k = 0; k < visible_faces.size(); k++)
		{
			for (size_t j = 0; j < visible_faces[k]->edges.size(); j++)
			{
				if ( visible_faces[k]->edges[j]->faces[0]->visible && 
					visible_faces[k]->edges[j]->faces[1]->visible )
				{
					tobe_deleted_edges.push_back(visible_faces[k]->edges[j]);
				}
				else
				{
					//Atleast one edge in visible faces is visible
					border_edges.push_back(visible_faces[k]->edges[j]);
				}
			}
		}

		std::vector<Face*> new_faces;
		std::vector<Edge3d*> new_edges;

		const unsigned int new_size = border_edges.size();

		// We need to find the unique points in border edges.
		std::list<Vertex3d*> unque_vertices;
		for (size_t j = 0; j < new_size; j++)
		{	
			unque_vertices.push_back(border_edges[j]->vertices[0]);
			unque_vertices.push_back(border_edges[j]->vertices[1]);
		}

		// Due to below sorting we cannot rely on the created order for adding faces to the edge. So we need to explicitly check
		// for incident faces in that case.
		unque_vertices.sort();
		unque_vertices.unique( [](Vertex3d* a, Vertex3d* b){ return *(a->point) == *(b->point); } );
		std::list<Vertex3d*>::iterator it;

		for (size_t j = 0; j < new_size; j++)
		{
			it = unque_vertices.begin();
			std::advance(it, j);
			
			// New faces and edges for new convex polyhedron
			new_edges.push_back(new Edge3d(*it, vertices[i]));
			new_faces.push_back(new Face(border_edges[j]->vertices[0], vertices[i], border_edges[j]->vertices[1]));
		
			//Add new face referece to borader edges
			if (border_edges[j]->faces[0]->visible)
			{
				border_edges[j]->faces[0] = new_faces[new_faces.size() - 1];
			}
			else
			{
				border_edges[j]->faces[1] = new_faces[new_faces.size() - 1];
			}	
		}

		//Adjust the faces normals
		for (size_t j = 0; j < new_size; j++)
		{
			adjust_normal(new_faces[j], point_ref);
		}

		// Added faces for edges
		// Based on our assumptions we must have exactly two faces incident wiht each edge
		for (size_t j = 0; j < new_edges.size(); j++)
		{
			std::vector<Face*> incident_faces;

			for (size_t k = 0; k < new_faces.size(); k++)
			{
				if(incident_face(new_faces[k],new_edges[j]))
					incident_faces.push_back(new_faces[k]);
			}

			new_edges[j]->faces[0] = incident_faces[0];
			new_edges[j]->faces[1] = incident_faces[1];
		}

		// Added edges for faces
		for (size_t j = 0; j < new_size; j++)
		{
			new_faces[j]->addEdge(border_edges[j]);
			for (size_t k = 0; k < new_edges.size(); k++)
			{
				auto r1 = std::find(new_faces[j]->vertices.begin(), 
					new_faces[j]->vertices.end(), new_edges[k]->vertices[0]);
				auto r2 = std::find(new_faces[j]->vertices.begin(), 
					new_faces[j]->vertices.end(), new_edges[k]->vertices[1]);

				if (incident_face(new_faces[j], new_edges[k]))
					new_faces[j]->addEdge(new_edges[k]);
			}
		}

		// Deleted the edges from to be deleted list
		for (size_t k = 0; k< tobe_deleted_edges.size(); k++)
		{
			for (size_t j = 0; j < edges.size(); j++)
			{
				if (*tobe_deleted_edges[k] == *edges[j])
					edges.erase(edges.begin() + j);
			}
		}

		// Delete the visible faces. 
		for (size_t k = 0; k < visible_faces.size(); k++)
		{
			for (size_t j = 0; j < faces.size(); j++)
			{
				if( visible_faces[k] == faces[j])
					faces.erase(faces.begin()+j);
			}
		}

		faces.insert(faces.end(), new_faces.begin(), new_faces.end());
		edges.insert(edges.end(), new_edges.begin(), new_edges.end());
	}
}

void jmk::convexhull3DQuickhull(std::vector<Point3d>& _points, Polygon& _results)
{
}
