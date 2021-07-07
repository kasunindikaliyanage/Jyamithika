#include "Polygon.h"

using namespace jmk;

Point3d Vertex::getPoint() const
{
	return this->point;
}

Polygon::Polygon(std::vector<Point3d> _point_list)
{
	for (Point3d& _point : _point_list)
	{
		vertex_list.push_back(Vertex(_point));
	}

	const int size = vertex_list.size();

	for (size_t i = 0; i < size ; i++)
	{
		vertex_list[i].next = &vertex_list[(i + 1)%size];
		
		if (i != 0)
			vertex_list[i].prev = &vertex_list[i - 1];
		else
			vertex_list[i].prev = &vertex_list[size -1];
	}
}

std::vector<Point3d> Polygon::getPoints()
{
	std::vector<Point3d> point_list;
	for (const Vertex& vertex : vertex_list)
	{
		point_list.push_back(vertex.point);
	}
	return point_list;
}
