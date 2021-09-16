#include "KDTree.h"

using namespace jmk;

void KDTree::traverse(KDNode* _node, std::list<Vector2f>& _list) {
	if (!_node)
		return;
	traverse(_node->left, _list);
	if (isALeaf(_node))
		_list.push_back(_node->data);
	traverse(_node->right, _list);
}

bool KDTree::isALeaf(KDNode* _node) {
	if (!_node->left && !_node->right)
		return true;
	return false;
}

bool KDTree::isIntersect(const KDRange& r1, const KDRange& r2) {
	if (r1.x_max < r2.x_min || r1.x_min > r2.x_max)
		return false;
	if (r1.y_max < r2.y_min || r1.y_min > r2.y_max)
		return false;
	return true;
}

bool KDTree::isContained(const KDRange& r1, const KDRange& r2) {
	if (r1.x_min >= r2.x_min && r1.x_max <= r2.x_max && r1.y_min >= r2.y_min && r1.y_max <= r2.y_max)
		return true;
	return false;
}

bool KDTree::isInRange(const Vector2f& p, const KDRange& r) {
	if (p[X] >= r.x_min && p[X] <= r.x_max && p[Y] >= r.y_min && p[Y] <= r.y_max)
		return true;
	return false;
}

static float sqrd_distance(const Vector2f& v1, const Vector2f& v2) {
	return (v1[X] - v2[X]) * (v1[X] - v2[X]) + (v1[Y] - v2[Y]) * (v1[Y] - v2[Y]);
}

KDTree::KDRange KDTree::intersection(const KDRange& r1, const KDRange& r2) {
	if (!isIntersect(r1, r2))
		return INVALID_RANGE;
	if (isContained(r1, r2))
		return r1;
	if (isContained(r2, r1))
		return r2;

	auto max_x_min = r1.x_min > r2.x_min ? r1.x_min : r2.x_min;
	auto min_x_max = r1.x_max < r2.x_max ? r1.x_max : r2.x_max;
	auto max_y_min = r1.y_min > r2.y_min ? r1.y_min : r2.y_min;
	auto min_y_max = r1.x_max < r2.x_max ? r1.x_max : r2.x_max;

	return KDRange{ max_x_min, min_x_max, max_y_min, min_y_max };
}

KDTree::KDNode* jmk::KDTree::constructKDTree(std::list<Vector2f>& _data, uint32_t _depth)
{
	auto size = _data.size();
	if (size == 1)
		return new KDNode(_data.front());
	if (_depth % 2 == 0)
		_data.sort([](Vector2f a, Vector2f b) { return (a[X] < b[X]); });
	else {
		_data.sort([](Vector2f a, Vector2f b) { return (a[Y] < b[Y]); });
	}

	auto mid = size / 2;
	auto mid_ptr = _data.begin();
	std::advance(mid_ptr, mid);
	auto left_list = std::list<Vector2f>(_data.begin(), mid_ptr);
	auto right_list = std::list<Vector2f>(mid_ptr, _data.end());

	auto left_child = constructKDTree(left_list, _depth + 1);
	auto right_child = constructKDTree(right_list, _depth + 1);
	return new KDNode((*mid_ptr)[_depth % 2], left_child, right_child);
}

void KDTree::searchKDTree(KDNode* _node, KDRange _range, std::list<Vector2f>& _list) {
	if (isALeaf(_node)) {
		if (isInRange(_node->data, _range))
			_list.push_back(_node->data);
	}
	else {
		if (isContained(_node->left->boundary, _range)) {
			traverse(_node->left, _list);
		}
		else if (isIntersect(_node->left->boundary, _range)) {
			searchKDTree(_node->left, _range, _list);
		}

		if (isContained(_node->right->boundary, _range)) {
			traverse(_node->right, _list);
		}
		else if (isIntersect(_node->right->boundary, _range)) {
			searchKDTree(_node->right, _range, _list);
		}
	}
}

void KDTree::preprocessBoundaries(KDNode* _node, bool _isEvenDepth) {
	if (!_node || isALeaf(_node))
		return;

	if (_isEvenDepth) {
		if (_node->left) {
			_node->left->boundary = _node->boundary;
			_node->left->boundary.x_max = _node->value;
			preprocessBoundaries(_node->left, !_isEvenDepth);
		}

		if (_node->right) {
			_node->right->boundary = _node->boundary;
			_node->right->boundary.x_min = _node->value;
			preprocessBoundaries(_node->right, !_isEvenDepth);
		}
	}
	else {
		if (_node->left) {
			_node->left->boundary = _node->boundary;
			_node->left->boundary.y_max = _node->value;
			preprocessBoundaries(_node->left, !_isEvenDepth);
		}

		if (_node->right) {
			_node->right->boundary = _node->boundary;
			_node->right->boundary.y_min = _node->value;
			preprocessBoundaries(_node->right, !_isEvenDepth);
		}
	}
}

void jmk::KDTree::nearestNeighbour(KDNode* _node, const Vector2f& _value, float& _current_distance
	, bool _even_depth, Vector2f& _current_nn){
	if (isALeaf(_node)) {
		auto distance = sqrd_distance(_value, _node->data);
		if (distance < _current_distance) {
			_current_distance = distance;
			_current_nn = _node->data;
		}
	}
	else {
		auto index = _even_depth ? X : Y;
		if (_value[index] < _node->value) {
			nearestNeighbour(_node->left, _value, _current_distance, !_even_depth, _current_nn);
			if (fabs(_value[index] - _node->value) < _current_distance)
				nearestNeighbour(_node->right, _value, _current_distance, !_even_depth, _current_nn);
		}
		else {
			nearestNeighbour(_node->right, _value, _current_distance, !_even_depth, _current_nn);
			if (fabs(_value[index] - _node->value) < _current_distance)
				nearestNeighbour(_node->left, _value, _current_distance, !_even_depth, _current_nn);
		}
	}
}

void KDTree::Search(const float x_min, const float x_max, const float y_min,
	const float y_max, std::list<Vector2f>& _list) {
	KDRange range{ x_min, x_max, y_min, y_max };
	searchKDTree(root, range, _list);
}

void KDTree::Traverse(std::list<Vector2f>& _list) {
	traverse(root, _list);
}

void KDTree::getSplitLineData(KDNode* _node, std::list<Vector2f>& _data, bool _isEvenDepth) {
	if (!_node)
		return;

	if (_isEvenDepth) {
		_data.push_back(Vector2f(_node->value, root->boundary.y_min));
		_data.push_back(Vector2f(_node->value, root->boundary.y_max));
	}
	else {
		_data.push_back(Vector2f(_node->boundary.x_min, _node->value));
		_data.push_back(Vector2f(_node->boundary.y_max, _node->value));
	}

	getSplitLineData(_node->left, _data, !_isEvenDepth);
	getSplitLineData(_node->right, _data, !_isEvenDepth);
}

void KDTree::NearestNeighbour(const Vector2f& _search_node, Vector2f& _ref_node) {
	float current_val = FLT_MAX;
	nearestNeighbour(root, _search_node, current_val, true, _ref_node);
}