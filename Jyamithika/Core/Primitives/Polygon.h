#pragma once
#include <vector>

#include "Point.h"

namespace jmk {
	class Vertex 
	{
		Point3d point;
		Vertex* next = nullptr;
		Vertex* prev = nullptr;

		friend class Polygon;

	public:
		Vertex(Point3d& _point, Vertex* _next = nullptr, Vertex* _prev = nullptr) : point(_point), next(_next), prev(_prev) {}
		
		Point3d getPoint() const;
	};

	struct Edge2d {
		Point2d p1;
		Point2d p2;
		Point2d fp1;
		Point2d fp2;
	public:
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


	void merge(Polygon& poly1, Polygon& poly2, Polygon& final_poly);
}