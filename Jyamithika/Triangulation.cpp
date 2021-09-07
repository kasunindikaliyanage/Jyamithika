#include "Triangulation.h"
#include <algorithm>
#include <stack>
#include <map>
#include "Core\GeoUtils.h"

using namespace jmk;

static bool is_in_same_chain(Vertex2dDCEL* a, Vertex2dDCEL* b)
{
	if (a->incident_edge->prev->origin == b || a->incident_edge->next->origin == b)
		return true;
	return false;
}

// Vertex should be in a monotone polygon
// a[Y] <= b[Y]
static bool valid_diagonal_DCEL(Vertex2dDCEL* a, Vertex2dDCEL* b)
{
	// TODO this is wrong. have to modify. Here only incode test would be needed.
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

	for (size_t i = 2; i < vertices.size() -1; i++) {
		if (!is_in_same_chain(vertices[i], vertex_stack.top())){
			bool popped = false;
			Vertex2dDCEL* top = vertex_stack.top();

			while (!vertex_stack.empty()) {
				if (vertex_stack.size() > 1)
					poly->split(vertices[i], vertex_stack.top());
				vertex_stack.pop();
				//popped = true;
			}

			//if(popped)
			if(top)
				vertex_stack.push(top);
			vertex_stack.push(vertices[i]);
		}
		else{
			Vertex2dDCEL* top_before_ops = vertex_stack.top();
			vertex_stack.pop();
			Vertex2dDCEL* last_vertex_ptr = nullptr;
			Vertex2dDCEL* last_diagonal_vert = nullptr;

			//bool diagonal_added = false;

			while (vertex_stack.size() != 0){
				last_vertex_ptr = vertex_stack.top();
				if (valid_diagonal_DCEL(vertices[i], last_vertex_ptr)) {
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
				vertex_stack.push(top_before_ops);
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

	vertex_stack.pop();
}

void jmk::triangulate_monotone(Polygon2d* poly)
{
	std::vector<Vertex2dDCEL*> vertices = poly->getVertexList();
	triangulate(poly, vertices);
}

static void initialize_ear_status(Polygon2dSimple* polygon)
{
	Vertex2dSimple* v0, * v1, * v2;

	auto vertices = polygon->getVertcies();
	v1 = vertices[0];
	do {
		v0 = v1->prev;
		v2 = v1->next;
		v1->is_ear = isDiagonal(v0,v2);
		v1 = v1->next;
	} while (v1 != vertices[0]);
}

void jmk::triangulate_earclipping(Polygon2dSimple* poly, std::vector<Edge2dSimple>& edge_list){
	
	initialize_ear_status(poly);

	auto vertex_list = poly->getVertcies();
	int no_vertex_to_process = vertex_list.size();
	
	Vertex2dSimple* v0, * v1, * v2, * v3, * v4;
	int index = 0;

	while (no_vertex_to_process > 3){	
		for (size_t i = 0; i < vertex_list.size(); i++)
		{
			v2 = vertex_list[i];
			if (v2->is_ear && !v2->is_processed) {
				v3 = v2->next;
				v4 = v3->next;
				v1 = v2->prev;
				v0 = v1->prev;

				edge_list.push_back(Edge2dSimple(v1->point, v3->point));
				v2->is_processed = true;

				v1->next = v3;
				v3->prev = v1;

				v1->is_ear = isDiagonal(v0, v3, nullptr);
				v3->is_ear = isDiagonal(v1, v4, nullptr);

				no_vertex_to_process--;
				index++;
				break;
			}
		}
	}
}
