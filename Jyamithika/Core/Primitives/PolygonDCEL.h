#pragma once

#include <vector>
#include "Point.h"

namespace jmk {
	template<class type, size_t dim >
	struct EdgeDCEL;
	
	template<class type, size_t dim>
	struct FaceDCEL;

	template<class type = float, size_t dim = DIM3 >
	struct VertexDCEL {
		Vector<float, dim> point;								// Coordinates of the vertex
		EdgeDCEL<type, dim>* incident_edge = nullptr;			// Incident edge to the vertex

		VertexDCEL(Vector<type, dim>& _point) : point(_point) {}
	};

	template<class type = float, size_t dim = DIM3 >
	struct EdgeDCEL {
		VertexDCEL<type, dim>* origin;
		EdgeDCEL<type, dim>* twin;
		EdgeDCEL<type, dim>* next;
		EdgeDCEL<type, dim>* prev;
		FaceDCEL<type, dim>* incident_face;

		EdgeDCEL(VertexDCEL<type, dim>* _origin) :origin(_origin) {};

		VertexDCEL<type, dim>* destination() {
			return twin->origin;
		}
	};

	template<class type = float, size_t dim = DIM3 >
	struct FaceDCEL {
		EdgeDCEL<type, dim>* outer = nullptr;		// Pointer to one of the counter clockwise edges
		std::vector<EdgeDCEL<type, dim>*> inner;	// Pointer to clockwise edges which represnt holes.
	};

	template<class type = float, size_t dim = DIM3 >
	class PolygonDCEL {
		typedef Vector<type,dim> VectorNf;
		std::vector<VertexDCEL<type, dim>*> vertex_list;
		std::vector<EdgeDCEL<type, dim>*> edge_list;
		std::vector<FaceDCEL<type, dim>*> face_list;
	public:
		
		// Construct the double connected edge list using the given points.
		// Assume the given points list is for polygon and have counter clockwise order 
		explicit PolygonDCEL(std::vector<VectorNf>&);

		// Insert an edge between virtices _v1 and _v2 given that the edge lies completely inside the orginal polygon
		bool split(VertexDCEL<type, dim>* _v1, VertexDCEL<type, dim>* _v2);

		// Join the two faces sepated by edge between _v1 and _v2
		bool join(VertexDCEL<type, dim>* _v1, VertexDCEL<type, dim>* _v2);

		// Return the all the vertices across all the faces.
		std::vector<VertexDCEL<type, dim>*> getVertexList();

		// Return all faces
		std::vector<FaceDCEL<type, dim>*> getFaceList();
	};

	template<class type, size_t dim>
	inline PolygonDCEL<type, dim>::PolygonDCEL(std::vector<VectorNf>& _points)
	{
		int size = _points.size();
		// Polygon should have atleast tree vertices.
		if (size < 3)
			return;

		for (size_t i = 0; i < _points.size(); i++) {
			vertex_list.push_back(new VertexDCEL<type, dim>(_points[i]));
		}

		for (size_t i = 0; i < _points.size() - 1; i++) {
			edge_list.push_back(new EdgeDCEL(vertex_list[i]));
			edge_list.push_back(new EdgeDCEL(vertex_list[i + 1]));

			// Each iteration added 2 edges. So we have to multiple the index by 2 to get the ith edge
			int hfindex1 = i * 2, hfindex2 = hfindex1 + 1;

			vertex_list[i]->incident_edge = edge_list[hfindex1];

			edge_list[hfindex1]->twin = edge_list[hfindex2];
			edge_list[hfindex2]->twin = edge_list[hfindex1];
		}
								
		edge_list.push_back(new EdgeDCEL(vertex_list.back()));
		edge_list.push_back(new EdgeDCEL(vertex_list.front()));
		edge_list[edge_list.size() - 2]->twin = edge_list[edge_list.size() - 1];
		edge_list[edge_list.size() - 1]->twin = edge_list[edge_list.size() - 2];

		// Set the prev and next for the element middle of the list ( 2 : size- 2)
		for (size_t i = 2; i < edge_list.size()-2; i++) {
			
			if (i % 2 == 0)
			{
				edge_list[i]->next = edge_list[i + 2];	
				edge_list[i]->prev = edge_list[i - 2];
			}
			else
			{
				edge_list[i]->next = edge_list[i - 2];
				edge_list[i]->prev = edge_list[i + 2];
			}
		}

		edge_list[0]->next = edge_list[2];
		edge_list[0]->prev = edge_list[edge_list.size()-2];
		edge_list[1]->next = edge_list[edge_list.size() - 1];
		edge_list[1]->prev = edge_list[3];

		edge_list[edge_list.size() - 2]->next = edge_list[0];
		edge_list[edge_list.size() - 2]->prev = edge_list[edge_list.size() - 4];
		edge_list[edge_list.size() - 1]->next = edge_list[1];
		edge_list[edge_list.size() - 1]->prev = edge_list[edge_list.size() - 3];

		// Configure the faces.
		FaceDCEL<type, dim>* f1 = new FaceDCEL<type, dim>();
		//FaceDCEL<type, dim>* f2 = new FaceDCEL<type, dim>();

		f1->outer = edge_list[0];
		//// f2 is unbounded face which wrap the f1. So f1 is a hole in f2. So have clockwise edges in innder edge list
		//f2->inner.push_back(edge_list[1]);

		f1->outer->incident_face = f1;
		EdgeDCEL* edge = f1->outer->next;
		while (edge != f1->outer)
		{
			edge->incident_face = f1;
			edge = edge->next;
		}

		//f2->inner[0]->incident_face = f2;
		//edge = f2->inner[0]->next;
		//while (edge != f2->inner[0])
		//{
		//	edge->incident_face = f2;
		//	edge = edge->next;
		//}
	}

	template<class type, size_t dim>
	static void getEdgesWithSamefaceAndGivenOrigins(VertexDCEL<type, dim>* _v1, VertexDCEL<type, dim>* _v2,
		EdgeDCEL<type, dim>* edge_leaving_v1, EdgeDCEL<type, dim>* edge_leaving_v2)
	{
		
	
	}

	template<class type, size_t dim>
	inline bool PolygonDCEL<type, dim>::split(VertexDCEL<type, dim>* _v1, VertexDCEL<type, dim>* _v2)
	{
		// Find two edges with given points as origins and are in same face.
		EdgeDCEL<type, dim>* edge_oriV1, * edge_oriV2;

		getEdgesWithSamefaceAndGivenOrigins(_v1, _v2, edge_oriV1, edge_oriV2);

		auto half_edge1 = new EdgeDCEL(_v1);
		auto half_edge2 = new EdgeDCEL(_v2);

		half_edge1->twin = half_edge2;
		half_edge1->prev = edge_oriV1->prev;
		half_edge1->next = edge_oriV2;
		half_edge1->prev->next = half_edge1;
		half_edge1->next->prev = half_edge1;

		half_edge2->twin = half_edge1;
		half_edge2->prev = edge_oriV2->prev;
		half_edge2->next = edge_oriV1;
		half_edge2->prev->next = half_edge2;
		half_edge2->next->prev = half_edge2;

		FaceDCEL<type, dim>* new_face = new FaceDCEL();
		new_face->outer = half_edge1;
		half_edge1->incident_face = new_face;
		auto temp_edge = half_edge1->next;
		while (temp_edge != half_edge1) {
			temp_edge->incident_face = new_face;
			temp_edge = temp_edge->next;
		}

		face_list.push_back(new_face);

		//Rest of the edges already have old_dace as incident face. So no need to iterate here.
		half_edge2->incident_face = edge_oriV1->incident_face;;

	}

	template<class type, size_t dim>
	inline bool PolygonDCEL<type, dim>::join(VertexDCEL<type, dim>* _v1, VertexDCEL<type, dim>* _v2)
	{
		return false;
	}

	template<class type, size_t dim>
	inline std::vector<VertexDCEL<type, dim>*> PolygonDCEL<type, dim>::getVertexList()
	{
		return vertex_list;
	}

	template<class type, size_t dim>
	inline std::vector<FaceDCEL<type, dim>*> PolygonDCEL<type, dim>::getFaceList()
	{
		return face_list;
	}
}