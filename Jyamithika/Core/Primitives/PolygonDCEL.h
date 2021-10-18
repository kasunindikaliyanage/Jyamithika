#pragma once

#include <vector>
#include <iostream>
#include "Point.h"

namespace jmk {
	static int _id = 1;

	template<class type, size_t dim >
	struct EdgeDCEL;
	
	template<class type, size_t dim>
	struct FaceDCEL;

	template<class type = float, size_t dim = DIM3 >
	struct VertexDCEL {
		Vector<float, dim> point;								// Coordinates of the vertex
		EdgeDCEL<type, dim>* incident_edge = nullptr;			// Incident edge to the vertex

		VertexDCEL(Vector<type, dim>& _point) : point(_point) {}

		void print() {
			std::cout << "(" << point[X] << "," << point[Y] << ") \n";
		}
	};

	template<class type = float, size_t dim = DIM3 >
	struct EdgeDCEL {
		VertexDCEL<type, dim>* origin = nullptr;
		EdgeDCEL<type, dim>* twin = nullptr;
		EdgeDCEL<type, dim>* next = nullptr;
		EdgeDCEL<type, dim>* prev = nullptr;
		FaceDCEL<type, dim>* incident_face = nullptr;
		int id;

		EdgeDCEL() { id = -1; }

		EdgeDCEL(VertexDCEL<type, dim>* _origin) :origin(_origin) {
			id = _id++;
		};

		VertexDCEL<type, dim>* destination() {
			return twin->origin;
		}

		void print()
		{
			std::cout << "This point pointer" << this << "\n";
			std::cout << "Origin : "; this->origin->print();
			std::cout << "Twin pointer" << this->twin << "\n";
			std::cout << "Next pointer" << this->next << "\n";
			std::cout << "Prev pointer" << this->prev << "\n";
		}
	};

	template<class type = float, size_t dim = DIM3 >
	struct FaceDCEL {
		EdgeDCEL<type, dim>* outer = nullptr;		// Pointer to one of the counter clockwise edges
		std::vector<EdgeDCEL<type, dim>*> inner;	// Pointer to first halfedges which represnt holes.

		void print(){
			if (outer)
			{
				auto edge_ptr = outer;
				auto next_ptr = outer->next;

				edge_ptr->origin->print();
				while (next_ptr!=edge_ptr) {
					next_ptr->origin->print();
					next_ptr = next_ptr->next;
				}
			}
		}

		std::vector<EdgeDCEL<type, dim>*> getEdgeList(){
			std::vector<EdgeDCEL<type, dim>*> edge_list;
			if (outer){
				auto edge_ptr = outer;
				auto next_ptr = outer->next;
				edge_list.push_back(edge_ptr);
				edge_ptr->origin->print();
				while (next_ptr != edge_ptr) {
					edge_list.push_back(next_ptr);
					next_ptr = next_ptr->next;
				}
			}
			return edge_list;
		}

		std::vector<Vector<float, dim>> getPoints()
		{
			std::vector<Vector<float, dim>> point_list;
			if (outer) {
				auto edge_ptr = outer;
				auto next_ptr = outer->next;
				point_list.push_back(edge_ptr->origin->point);
				while (next_ptr != edge_ptr) {
					point_list.push_back(next_ptr->origin->point);
					next_ptr = next_ptr->next;
				}
			}
			return point_list;
		}
	};

	template<class type = float, size_t dim = DIM3 >
	class PolygonDCEL {
		typedef Vector<type,dim> VectorNf;
		std::vector<VertexDCEL<type, dim>*> vertex_list;
		std::vector<EdgeDCEL<type, dim>*> edge_list;
		std::vector<FaceDCEL<type, dim>*> face_list;

		EdgeDCEL<type, dim>* empty_edge = new EdgeDCEL<type, dim>();
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

		std::vector<EdgeDCEL<type, dim>*> getEdgeList();

		VertexDCEL<type, dim>* getVertex(VectorNf&);

		void getEdgesWithSamefaceAndGivenOrigins(VertexDCEL<type, dim>* _v1, VertexDCEL<type, dim>* _v2,
			EdgeDCEL<type, dim>** edge_leaving_v1, EdgeDCEL<type, dim>** edge_leaving_v2);
	};

	typedef VertexDCEL<float, 2U>	Vertex2dDCEL;
	typedef EdgeDCEL<float, 2U>	Edge2dDCEL;
	typedef PolygonDCEL<float, 2U>	Polygon2d;

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

		for (size_t i = 0; i <= vertex_list.size() - 2; i++) {
			auto hfedge = new EdgeDCEL<type, dim>(vertex_list[i]);
			auto edge_twin = new EdgeDCEL<type, dim>(vertex_list[i + 1]);
			
			vertex_list[i]->incident_edge = hfedge;
			
			hfedge->twin = edge_twin;
			edge_twin->twin = hfedge;
			
			edge_list.push_back(hfedge);
			edge_list.push_back(edge_twin);
		}
		
		auto hfedge = new EdgeDCEL<type, dim>(vertex_list.back());
		auto edge_twin = new EdgeDCEL<type, dim>(vertex_list.front());
		
		hfedge->twin = edge_twin;
		edge_twin->twin = hfedge;
		edge_list.push_back(hfedge);
		edge_list.push_back(edge_twin);

		vertex_list[vertex_list.size() - 1]->incident_edge = hfedge;

		// Set the prev and next for the element middle of the list ( 2 : size- 2)
		for (size_t i = 2; i <= edge_list.size()-3; i++) {
			
			if (i % 2 == 0) // Even case. Counter clockwise edges
			{
				edge_list[i]->next = edge_list[i + 2];	
				edge_list[i]->prev = edge_list[i - 2];
			}
			else           // Odd case. Clockwise edges
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
		edge_list[edge_list.size() - 1]->next = edge_list[edge_list.size() - 3];
		edge_list[edge_list.size() - 1]->prev = edge_list[1];

		// Configure the faces.
		FaceDCEL<type, dim>* f1 = new FaceDCEL<type, dim>();
		FaceDCEL<type, dim>* f2 = new FaceDCEL<type, dim>();

		f1->outer = edge_list[0];
		// f2 is unbounded face which wrap the f1. So f1 is a hole in f2. So have clockwise edges in innder edge list
		f2->inner.push_back(edge_list[1]);

		face_list.push_back(f1);
		face_list.push_back(f2);

		f1->outer->incident_face = f1;
		EdgeDCEL<type, dim>* edge = f1->outer->next;
		while (edge != f1->outer)
		{
			edge->incident_face = f1;
			edge = edge->next;
		}

		// f2->inner has halfedges connect in clockwise order
		f2->inner[0]->incident_face = f2;
		edge = f2->inner[0]->next;
		while (edge != f2->inner[0])
		{
			edge->incident_face = f2;
			edge = edge->next;
		}

	}

	template<class type, size_t dim>
	inline void PolygonDCEL<type, dim>::getEdgesWithSamefaceAndGivenOrigins(
		VertexDCEL<type, dim>* _v1,VertexDCEL<type, dim>* _v2,
		EdgeDCEL<type, dim>** edge_leaving_v1, EdgeDCEL<type, dim>** edge_leaving_v2)
	{
		std::vector<EdgeDCEL<type, dim>*> edges_with_v1_ori, edges_with_v2_ori;
		
		// Get all the edges with orgin _v1
		auto v1_inci_edge = _v1->incident_edge;
		edges_with_v1_ori.push_back(v1_inci_edge);
		
		auto next_edge = v1_inci_edge->twin->next;
		while (next_edge != v1_inci_edge) {
			edges_with_v1_ori.push_back(next_edge);
			next_edge = next_edge->twin->next;
		}

		// Get all the edges with orgin _v2
		auto v2_inci_edge = _v2->incident_edge;
		edges_with_v2_ori.push_back(v2_inci_edge);

		next_edge = v2_inci_edge->twin->next;
		while (next_edge != v2_inci_edge)
		{
			edges_with_v2_ori.push_back(next_edge);
			next_edge = next_edge->twin->next;
		}

		// Get two edges, one with origin v1 and other with origin v2 and incident to same face
		for ( auto ev1 : edges_with_v1_ori){
			for ( auto ev2 : edges_with_v2_ori){
				if (ev1->incident_face->outer != nullptr) {
					if (ev1->incident_face == ev2->incident_face) {
						*edge_leaving_v1 = ev1;
						*edge_leaving_v2 = ev2;
						return;
					}
				}
			}
		}
	}

	template<class type, size_t dim>
	inline bool PolygonDCEL<type, dim>::split(VertexDCEL<type, dim>* _v1, VertexDCEL<type, dim>* _v2)
	{
		// Find two edges with given points as origins and are in same face.
		EdgeDCEL<type, dim>* edge_oriV1;
		EdgeDCEL<type, dim>* edge_oriV2;
		getEdgesWithSamefaceAndGivenOrigins(_v1, _v2, &edge_oriV1, &edge_oriV2);
		
		if (edge_oriV1->id == -1 || edge_oriV2->id == -1)
			return false;						// Cannot find a edges with same face with ori _v1, _v2

		// If the vertices are adjucent we can return.
		if (edge_oriV1->next->origin == _v2 || edge_oriV1->prev->origin == _v2)
			return false;

		// Later we can delete this entry
		FaceDCEL<type, dim>* previous_face = edge_oriV1->incident_face;

		auto half_edge1 = new EdgeDCEL<type, dim>(_v1);
		auto half_edge2 = new EdgeDCEL<type, dim>(_v2);

		half_edge1->twin = half_edge2;
		half_edge2->twin = half_edge1;
		half_edge1->next = edge_oriV2;
		half_edge2->next = edge_oriV1;

		half_edge1->prev = edge_oriV1->prev;
		half_edge2->prev = edge_oriV2->prev;

		half_edge1->next->prev = half_edge1;
		half_edge2->next->prev = half_edge2;
		half_edge1->prev->next = half_edge1;
		half_edge2->prev->next = half_edge2;

		FaceDCEL<type, dim>* new_face1 = new FaceDCEL<type, dim>();
		new_face1->outer = half_edge1;
		half_edge1->incident_face = new_face1;
		auto temp_edge = half_edge1->next;
		while (temp_edge != half_edge1) {
			temp_edge->incident_face = new_face1;
			temp_edge = temp_edge->next;
		}

		FaceDCEL<type, dim>* new_face2 = new FaceDCEL<type, dim>();
		new_face2->outer = half_edge2;
		half_edge2->incident_face = new_face2;
		temp_edge = half_edge2->next;
		while (temp_edge != half_edge2) {
			temp_edge->incident_face = new_face2;
			temp_edge = temp_edge->next;
		}

		face_list.push_back(new_face1);
		face_list.push_back(new_face2);

		auto itr = std::find(face_list.begin(), face_list.end(), previous_face);

		if (itr != face_list.end()) {
			face_list.erase(itr);
			delete previous_face;
		}

		return true;
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

	template<class type, size_t dim>
	inline std::vector<EdgeDCEL<type, dim>*> PolygonDCEL<type, dim>::getEdgeList()
	{
		return edge_list;
	}

	template<class type, size_t dim>
	inline VertexDCEL<type, dim>* PolygonDCEL<type, dim>::getVertex(VectorNf& _point)
	{
		for (size_t i = 0; i < vertex_list.size(); i++){
			if (_point == vertex_list[i]->point)
				return vertex_list[i];
		}
		return nullptr;
	}

	struct Vertex2DSortTBLR {
		bool operator()(Vertex2dDCEL* ref1, Vertex2dDCEL* ref2) {
			auto a = ref1->point;
			auto b = ref2->point;
			if ((a[Y] > b[Y])
				|| (a[Y] == b[Y]) && (a[X] < b[X]))
			{
				return true;
			}
			return false;
		}
	};
}