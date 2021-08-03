#include "Triangulation.h"
#include <algorithm>
#include <stack>

using namespace jmk;

static bool is_in_same_chain(Vertex2dDCEL* a, Vertex2dDCEL* b)
{
	if (a->incident_edge->prev->origin == b || a->incident_edge->next->origin == b)
		return true;
	return false;
}

// Vertex should be in a monotone polygon
// a[Y] <= b[Y]
static bool valid_diagonal(Vertex2dDCEL* a, Vertex2dDCEL* b)
{
	auto edge = a->incident_edge;
	// If the vertices are adjucent we can return.
	if (edge->next->origin == b || edge->prev->origin == b)
		return false;

	if (a->point[X] <= b->point[X]){
		edge = edge->prev;
		while (edge->origin != b){
			if (!left(a->point, b->point, edge->origin->point))
				return false;
			edge = edge->prev;
		}
	}
	else{
		edge = edge->next;
		while (edge->origin != b) {
			if (left(a->point, b->point, edge->origin->point))
				return false;
			edge = edge->next;
		}
	}

	return true;
}

static void triangulate(Polygon2d* poly, std::vector<Vertex2dDCEL*>& vertices)
{
	if (vertices.size() == 3)
		return;

	std::sort(vertices.begin(), vertices.end(), Vertex2DSortTBLR());
	std::stack<Vertex2dDCEL*> vertex_stack;
	vertex_stack.push(vertices[0]);
	vertex_stack.push(vertices[1]);

	for (int i = 2; i < vertices.size() -1; i++) {
		if (!is_in_same_chain(vertices[i], vertex_stack.top())){
			bool popped = false;
			Vertex2dDCEL* top = vertex_stack.top();

			while (vertex_stack.size() > 0) {
				if (vertex_stack.size() > 1)
					poly->split(vertices[i], vertex_stack.top());
				vertex_stack.pop();
				popped = true;
			}

			if(popped)
				vertex_stack.push(top);
			vertex_stack.push(vertices[i]);
		}
		else{
			Vertex2dDCEL* edge_in_poly = vertex_stack.top();
			vertex_stack.pop();
			Vertex2dDCEL* last_vertex_ptr = nullptr;
			Vertex2dDCEL* last_diagonal_vert = nullptr;

			//bool diagonal_added = false;

			while (vertex_stack.size() != 0){
				last_vertex_ptr = vertex_stack.top();
				if (valid_diagonal(vertices[i], last_vertex_ptr)) {
					poly->split(vertices[i], last_vertex_ptr);
					last_diagonal_vert = last_vertex_ptr;
				}
				else{
					break;
				}
				vertex_stack.pop();
			}

			if (last_diagonal_vert) {
				vertex_stack.push(last_diagonal_vert);
			}
			else {
				vertex_stack.push(edge_in_poly);
			}
			vertex_stack.push(vertices[i]);
		}
	}

	if (vertex_stack.size() != 0)
		vertex_stack.pop();

	auto size = vertices.size();
	while (vertex_stack.size() > 1)
	{
		poly->split(vertices[size - 1], vertex_stack.top());
		vertex_stack.pop();
	}
}

void jmk::triangulate_monotone(Polygon2d* poly)
{
	std::vector<Vertex2dDCEL*> vertices = poly->getVertexList();
	triangulate(poly, vertices);
}

//void jmk::triangulate_general(Polygon2d* poly)
//{
//	std::vector<Polygon2d> mono_polies;
//	get_monotone_polygons(poly, mono_polies);
//
//	std::vector<std::vector<Vertex2dDCEL*>> polygon_pieces_vertices;
//
//	for (auto face_ptr : poly->getFaceList()) {
//		auto first_edge_ptr = face_ptr->outer;
//		if (first_edge_ptr) {
//			std::vector<Vertex2dDCEL*> vertices;
//			vertices.push_back(first_edge_ptr->origin);
//
//			auto next_edge_ptr = first_edge_ptr->next;
//			while (next_edge_ptr != first_edge_ptr) {
//				vertices.push_back(next_edge_ptr->origin);
//				next_edge_ptr = next_edge_ptr->next;
//			}
//
//			polygon_pieces_vertices.push_back(vertices);
//		}
//	}
//
//	int count = 0;
//
//	for (auto vertices : polygon_pieces_vertices) {
//		triangulate(poly, vertices);
//		//std::cout << "Print the face for iteration" << count++ << std::endl;
//
//		//for (auto face_ptr : poly->getFaceList())
//		//{
//		//	face_ptr->print();
//		//	std::cout << std::endl;
//		//}
//	}
//}
