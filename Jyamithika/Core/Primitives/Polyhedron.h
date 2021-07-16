#pragma once
#include <vector>
#include "Polygon.h" // For vertex
#include "Plane.h"

namespace jmk {

	struct Face;

	struct Vertex3d {
		Point3d* point = nullptr;
		bool processed = false;

		Vertex3d() {}

		Vertex3d(Point3d* _point)
		{
			point = _point;
			processed = false;
		}
	};

	struct Edge3d {
		Vertex3d* vertices[2];
		Face* faces[2] = { nullptr, nullptr };

		Edge3d(Vertex3d* p1, Vertex3d* p2)
		{
			vertices[0] = p1;
			vertices[1] = p2;
		}

		bool operator==(const Edge3d& _other)
		{
			if (*vertices[0]->point == *_other.vertices[0]->point
				&& *vertices[1]->point == *_other.vertices[1]->point)
				return true;
			return false;
		}
	};

	struct Face {
		std::vector<Edge3d*> edges;
		std::vector<Vertex3d*> vertices;
		Planef plane;
		bool visible = false;
		bool normal_switch_needed = false;

		Face() {}

		Face(Vertex3d* p1, Vertex3d* p2, Vertex3d* p3)
		{
			vertices.push_back(p1);
			vertices.push_back(p2);
			vertices.push_back(p3);

			plane = Planef(*p1->point, *p2->point, *p3->point);
		}

		bool operator==(const Face& _other)
		{
			if (vertices.size() != _other.vertices.size())
				return false;

			bool ret = true;
			for (size_t i = 0; i < vertices.size(); i++)
			{
				if (vertices[i]->point != _other.vertices[i]->point )
				{
					ret = false;
					break;
				}
			}
			return ret;
		}

		void addEdge(Edge3d* edg_ptr)
		{
			edges.push_back(edg_ptr);
		}
	};

	class Polyhedron {
		std::vector<Face> faces;

	public:
	};

}