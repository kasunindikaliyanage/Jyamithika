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

	struct Vertex2d {
		Point2d point;
		Vertex2d* next = nullptr;
		Vertex2d* prev = nullptr;

		friend class Polygon2d;

	public:
		Vertex2d(Point2d& _point, Vertex2d* _next = nullptr, Vertex2d* _prev = nullptr) : point(_point), next(_next), prev(_prev) {}
	};

	struct Edge2d {
		Point2d p1;
		Point2d p2;
		Point2d fp1;
		Point2d fp2;
	public:
		Edge2d() {}

		Edge2d(Point2d _p1, Point2d _p2) :p1(_p1), p2(_p2)
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

	class Polygon2d {
		std::vector<Vertex2d> vertex_list;
	public:
		Polygon2d();

		// Construct the Polyhon with given point set
		Polygon2d(std::vector<Point2d> _point_list);

		void Insert(Point2d&);

		std::vector<Vertex2d> getVertcies();

		// Return the points list of underline vertices
		std::vector<Point2d> getPoints();

	};


	void merge(Polygon& poly1, Polygon& poly2, Polygon& final_poly);
}