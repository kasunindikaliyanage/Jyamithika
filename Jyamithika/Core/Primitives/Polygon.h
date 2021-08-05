#pragma once
#include <vector>

#include "Point.h"

namespace jmk {
	struct Vertex 
	{
		Point3d point;
		Vertex* next = nullptr;
		Vertex* prev = nullptr;
		int id = 0;

		friend class Polygon;

	public:
		Vertex(Point3d& _point, Vertex* _next = nullptr, Vertex* _prev = nullptr) : point(_point), next(_next), prev(_prev) {}
	};

	struct Vertex2dSimple {
		Point2d point;
		Vertex2dSimple* next = nullptr;
		Vertex2dSimple* prev = nullptr;
		bool is_ear = false;
		bool is_processed = false;

		friend class Polygon2dSimple;

	public:
		Vertex2dSimple(Point2d& _point, Vertex2dSimple* _next = nullptr, Vertex2dSimple* _prev = nullptr) : point(_point), next(_next), prev(_prev) {}
	};

	struct Edge2dSimple {
		Point2d p1;
		Point2d p2;
		Point2d fp1;
		Point2d fp2;
	public:
		Edge2dSimple() {}

		Edge2dSimple(Point2d _p1, Point2d _p2) :p1(_p1), p2(_p2)
		{}

	};

	class Polygon
	{
		std::vector<Vertex> vertex_list;
	public:
		Polygon();

		// Construct the Polyhon with given point set
		Polygon(std::vector<Point3d> _point_list );

		void Insert(Point3d&);

		// Return the points list of underline vertices
		std::vector<Point3d> getPoints();
	};

	class Polygon2dSimple {
		std::vector<Vertex2dSimple*> vertex_list;
	public:
		Polygon2dSimple();

		// Construct the Polyhon with given point set
		Polygon2dSimple(std::vector<Point2d> _point_list);

		Polygon2dSimple(Vertex2dSimple* root_vertex);

		void Insert(Point2d&);

		// If vert exist in the polygon, update the neighbour pointer accoidingly and remove the vertex.
		void RemoveVertex(Vertex2dSimple* vert);

		std::vector<Vertex2dSimple*> getVertcies();

		// Return the points list of underline vertices
		std::vector<Point2d> getPoints();

		int size();

	};

	void merge(Polygon& poly1, Polygon& poly2, Polygon& final_poly);
}