#include "Polygon.h"

#include "../GeoUtils.h"

using namespace jmk;

// Namespace classes and functions.

jmk::Polygon::Polygon()
{
}

// Polygon class function
Polygon::Polygon(std::vector<Point3d> _point_list)
{
	for (Point3d& _point : _point_list)
	{
		vertex_list.push_back(Vertex(_point));
	}

	const unsigned int size = vertex_list.size();

	for (size_t i = 0; i < size ; i++)
	{
		vertex_list[i].next = &vertex_list[(i + 1)%size];
		
		if (i != 0)
			vertex_list[i].prev = &vertex_list[i - 1];
		else
			vertex_list[i].prev = &vertex_list[size -1];
	}
}

void jmk::Polygon::Insert(Point3d& _point)
{
	vertex_list.push_back(Vertex(_point));
	int size = vertex_list.size();
	if (size>1)
	{
		vertex_list[size - 1].next = vertex_list[size - 2].next;
		vertex_list[size - 2].next = &vertex_list[size - 1];
		vertex_list[size - 1].prev = &vertex_list[size - 2];
		vertex_list[size - 1].next->prev = &vertex_list[size - 1];
	}
}

// Polygon class function
std::vector<Point3d> Polygon::getPoints()
{
	std::vector<Point3d> point_list;
	for (const Vertex& vertex : vertex_list)
	{
		point_list.push_back(vertex.point);
	}
	return point_list;
}


jmk::Polygon2dSimple::Polygon2dSimple()
{
}

jmk::Polygon2dSimple::Polygon2dSimple(std::vector<Point2d> _point_list)
{
	for (Point2d& _point : _point_list)
	{
		vertex_list.push_back(new Vertex2dSimple(_point));
	}

	const unsigned int size = vertex_list.size();

	for (size_t i = 0; i < size; i++)
	{
		vertex_list[i]->next = vertex_list[(i + 1) % size];

		if (i != 0)
			vertex_list[i]->prev = vertex_list[i - 1];
		else
			vertex_list[i]->prev = vertex_list[size - 1];
	}
}

jmk::Polygon2dSimple::Polygon2dSimple(Vertex2dSimple* root_vertex)
{
	vertex_list.push_back(root_vertex);
	auto temp = root_vertex->next;

	while (temp != root_vertex) {
		vertex_list.push_back(temp);
		temp = temp->next;
	}
}

void jmk::Polygon2dSimple::Insert(Point2d& _point)
{
	vertex_list.push_back(new Vertex2dSimple(_point));
	int size = vertex_list.size();
	if (size > 1)
	{
		vertex_list[size - 1]->next = vertex_list[size - 2]->next;
		vertex_list[size - 2]->next = vertex_list[size - 1];
		vertex_list[size - 1]->prev = vertex_list[size - 2];
		vertex_list[size - 1]->next->prev = vertex_list[size - 1];
	}
}

void jmk::Polygon2dSimple::RemoveVertex(Vertex2dSimple* _vert)
{
	auto itr = std::find(vertex_list.begin(), vertex_list.end(), _vert);
	if (itr != vertex_list.end())
	{
		vertex_list.erase(itr);
	}
}

std::vector<Point2d> jmk::Polygon2dSimple::getPoints()
{
	std::vector<Point2d> point_list;
	for (const Vertex2dSimple* vertex : vertex_list)
	{
		point_list.push_back(vertex->point);
	}
	return point_list;
}

int jmk::Polygon2dSimple::size()
{
	return vertex_list.size();
}

std::vector<Vertex2dSimple*> jmk::Polygon2dSimple::getVertcies()
{
	return this->vertex_list;
}


void jmk::merge(Polygon& poly1, Polygon& poly2, Polygon& final_poly)
{

}

