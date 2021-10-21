#pragma once

#include "../Core/Primitives/Point.h"
#include "../Core/Primitives/Segment.h"
#include "../Core/Primitives/Boundaries.h"

#include <vector>

namespace jmk{

#define NUM_POINTS 4
#define INE 0
#define INW 1
#define ISE 2
#define ISW 3


	struct QDTNode {
		QDTNode* NE = nullptr, * NW = nullptr, * SE = nullptr, * SW = nullptr;
		QDTNode* parent = nullptr;
		AABB box;
		Point2d point = DEFAULT_POINT_2D;
		bool isALeaf = false;
		//bool isAEmptyNode = false;
	};

	class QuadTree {

		QDTNode* root = nullptr;

	public:
		QuadTree() {}

		QuadTree(std::vector<Point2d> _points);

		QuadTree(std::vector<Point2d> _points, AABB& bounds);

		void BalanceTheTree();

		void GenerateMesh();

		void GetUniqueSegmentList(std::vector<Segment2d>& _segList);
	};
}