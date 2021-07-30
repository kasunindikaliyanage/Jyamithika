#pragma once

#include <vector>
#include "Point.h"

namespace jmk {

	struct Vertex2dDCEL {
		Point2d point;
		Edge2dDCEL* incident_edge;
	};

	struct Edge2dDCEL {
		Vertex2dDCEL* origin;
		Edge2dDCEL* twin;
		Edge2dDCEL* next;
		Edge2dDCEL* prev;
		Face2dDCEL* incident_face;
	};

	struct Face2dDCEL {
		Edge2dDCEL* outer;
		std::vector<Edge2dDCEL*> inner;
	};

	class Polygon2dDCEL {


	};


}