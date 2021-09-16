#pragma once
#include "../Core/Base/Core.h"
#include "../Core/Base/Vector.h"
#include "../Core/Primitives/Point.h"

#include <list>
#include <vector>
#include <algorithm>

namespace jmk {
	class KDTree {

		struct KDRange {
			float x_min, x_max, y_min, y_max;
		};

		struct KDNode {
			KDNode* left;
			KDNode* right;
			jmk::Vector2f data;
			float value = FLT_MIN;
			KDRange boundary{ -10, 10, -10, 10 };

			KDNode(float _value, KDNode* _left = nullptr, KDNode* _right = nullptr)
				:value(_value), left(_left), right(_right) {}

			KDNode(Vector2f _data, KDNode* _left = nullptr, KDNode* _right = nullptr)
				:data(_data), left(_left), right(_right) {}
		};

		KDNode* root = nullptr;
		KDRange default_bound{ -10, 10, -10, 10 };
		KDRange INVALID_RANGE{ 0, 0, 0, 0 };

		void traverse(KDNode* _node, std::list<Vector2f>& _list);

		bool isALeaf(KDNode* _node);
		bool isIntersect(const KDRange&, const KDRange&);

		// Check if r1 reside completely with in r2
		bool isContained(const KDRange& r1, const KDRange& r2);

		// Cehck if p is inside the r
		bool isInRange(const Vector2f& p, const KDRange& r);

		// Return the common region if r1 amd r2 intersect. If not return INVALID_RANGE
		KDRange intersection(const KDRange& r1, const KDRange& r2);

		KDNode* constructKDTree(std::list<Vector2f>& _data, uint32_t _depth);
		void searchKDTree(KDNode*, KDRange, std::list<Vector2f>&);

		// Calculate bounded rectangle for non leaf nodes in the KDTree
		void preprocessBoundaries(KDNode* _node, bool _isEvenDepth);

		void getSplitLineData(KDNode* _node, std::list<Vector2f>& _data, bool _isEvenDepth);

		void nearestNeighbour(KDNode* _node, const Vector2f& _value, float& current_distance, bool _even_depth, Vector2f& _current_nn);

	public:
		KDTree() {}

		KDTree(std::list<Vector2f> _data) {
			root = constructKDTree(_data, 0);
			root->boundary = default_bound;
			preprocessBoundaries(root, true);
		}

		void Search(const float x_min, const float x_max,
			const float y_min, const float y_max, std::list<Vector2f>&);

		void Traverse(std::list<Vector2f>&);

		void getSplitLineData(std::list<Vector2f>& _data) {
			if (root) {
				getSplitLineData(root, _data, true);
			}
		}

		void NearestNeighbour(const Vector2f&, Vector2f&);
	};
};