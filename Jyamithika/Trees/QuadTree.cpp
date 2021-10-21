#include "QuadTree.h"

jmk::QuadTree::QuadTree(std::vector<Point2d> _points)
{

}

static void partition(jmk::QDTNode* _parent, std::vector<jmk::Point2d> _points) {
	if (_points.size() == 0) {
		//_parent->isAEmptyNode = true;
		_parent->isALeaf = true;
		return;
	}
	else if (_points.size() == 1) {
		_parent->point = _points[0];
		_parent->isALeaf = true;
		return;
	}
	else {
		// Calcualte the boundaries of each child using the bounds of the parent
		auto box = _parent->box;
		float x_mid = (box.x_min + box.x_max) / 2;
		float y_mid = (box.y_min + box.y_max) / 2;

		jmk::AABB boxNW{ box.x_min, x_mid, y_mid, box.y_max };
		jmk::AABB boxNE{ x_mid, box.x_max, y_mid, box.y_max };
		jmk::AABB boxSW{ box.x_min, x_mid, box.y_min, y_mid };
		jmk::AABB boxSE{ x_mid, box.x_max, box.y_min, y_mid };

		// Paritition the _points to four list.
		std::vector<jmk::Point2d> pointsNW, pointsNE, pointsSW, pointsSE;
		for (auto& point : _points) {
			if (boxNW.isInside(point))
				pointsNW.push_back(point);
			else if (boxNE.isInside(point))
				pointsNE.push_back(point);
			else if (boxSW.isInside(point))
				pointsSW.push_back(point);
			else
				pointsSE.push_back(point);
		}

		// Create 4 childs and set the bounds
		auto nodeNW = new jmk::QDTNode();
		auto nodeNE = new jmk::QDTNode();
		auto nodeSW = new jmk::QDTNode();
		auto nodeSE = new jmk::QDTNode();

		nodeNW->box = boxNW;
		nodeNE->box = boxNE;
		nodeSW->box = boxSW;
		nodeSE->box = boxSE;

		// Do the recursive call.
		partition(nodeNW, pointsNW);
		partition(nodeNE, pointsNE);
		partition(nodeSW, pointsSW);
		partition(nodeSE, pointsSE);

		_parent->NW = nodeNW;
		_parent->NE = nodeNE;
		_parent->SW = nodeSW;
		_parent->SE = nodeSE;
		nodeNW->parent = _parent;
		nodeNE->parent = _parent;
		nodeSW->parent = _parent;
		nodeSE->parent = _parent;
	}
}

static void addBoundariesToTheList(jmk::QDTNode* _node, std::vector<jmk::Segment2d>& _segList) {
	//if (!_node->isALeaf && !_node->isAEmptyNode) {
	if (!_node->isALeaf) {
		auto box = _node->box;
		float x_mid = (box.x_min + box.x_max) / 2;
		float y_mid = (box.y_min + box.y_max) / 2;

		jmk::Point2d bot_mid(x_mid, box.y_min);
		jmk::Point2d top_mid(x_mid, box.y_max);
		jmk::Point2d left_mid(box.x_min, y_mid);
		jmk::Point2d right_mid(box.x_max, y_mid);

		_segList.push_back(jmk::Segment2d(bot_mid, top_mid));
		_segList.push_back(jmk::Segment2d(left_mid, right_mid));

		addBoundariesToTheList(_node->NW, _segList);
		addBoundariesToTheList(_node->NE, _segList);
		addBoundariesToTheList(_node->SW, _segList);
		addBoundariesToTheList(_node->SE, _segList);
	}
}

static jmk::QDTNode* northNeighbor(jmk::QDTNode* _node, jmk::QDTNode* _root) {
	if (_node == _root)
		return nullptr;

	if (_node == _node->parent->SW)
		return _node->parent->NW;

	if (_node == _node->parent->SE)
		return _node->parent->NE;

	auto u = northNeighbor(_node->parent, _root);
	//if (!u || u->isALeaf || u->isAEmptyNode)
	if (!u || u->isALeaf)
		return u;
	else if (_node == _node->parent->NW)
		return u->SW;
	else
		return u->SE;
}

static jmk::QDTNode* southNeighbor(jmk::QDTNode* _node, jmk::QDTNode* _root) {
	if (_node == _root)
		return nullptr;

	if (_node == _node->parent->NW)
		return _node->parent->SW;

	if (_node == _node->parent->NE)
		return _node->parent->SE;

	auto u = southNeighbor(_node->parent, _root);
	//if (!u || u->isALeaf || u->isAEmptyNode)
	if (!u || u->isALeaf)
		return u;
	else if (_node == _node->parent->SW)
		return u->NW;
	else
		return u->NE;
}

static jmk::QDTNode* eastNeighbor(jmk::QDTNode* _node, jmk::QDTNode* _root) {
	if (_node == _root)
		return nullptr;

	if (_node == _node->parent->NW)
		return _node->parent->NE;

	if (_node == _node->parent->SW)
		return _node->parent->SE;

	auto u = eastNeighbor(_node->parent, _root);
	//if (!u || u->isALeaf || u->isAEmptyNode)
	if (!u || u->isALeaf)
		return u;
	else if (_node == _node->parent->NE)
		return u->NW;
	else
		return u->SW;
}

static jmk::QDTNode* westNeighbor(jmk::QDTNode* _node, jmk::QDTNode* _root) {
	if (_node == _root)
		return nullptr;

	if (_node == _node->parent->NE)
		return _node->parent->NW;

	if (_node == _node->parent->SE)
		return _node->parent->SW;

	auto u = westNeighbor(_node->parent, _root);
	//if (!u || u->isALeaf || u->isAEmptyNode)
	if (!u || u->isALeaf)
		return u;
	else if (_node == _node->parent->NW)
		return u->NE;
	else
		return u->SE;
}

jmk::QuadTree::QuadTree(std::vector<Point2d> _points, AABB& bounds)
{
	if (_points.size() == 0)
		return;
	else {
		root = new jmk::QDTNode();
		root->box = bounds;
		partition(root, _points);
	}
}

void balancing(jmk::QDTNode* _node) {

}

static void getLeafNodes(jmk::QDTNode* _node, std::vector<jmk::QDTNode*>& _leafs) {
	if (!_node)
		return;
	//if (_node->isALeaf || _node->isAEmptyNode)
	if (_node->isALeaf)
		_leafs.push_back(_node);
	else {
		getLeafNodes(_node->NW, _leafs);
		getLeafNodes(_node->NE, _leafs);
		getLeafNodes(_node->SW, _leafs);
		getLeafNodes(_node->SE, _leafs);
	}
}

static bool isEndNode(jmk::QDTNode* _node) {
	//if (_node && (_node->isALeaf || _node->isAEmptyNode))
	if (_node && (_node->isALeaf))
		return true;
	return false;
}

static bool needToSplit(jmk::QDTNode* _node, jmk::QDTNode* root) {
	if (!_node)
		return false;
	
	auto nNbor = northNeighbor(_node, root);
	auto sNbor = southNeighbor(_node, root);
	auto wNbor = westNeighbor(_node, root);
	auto eNbor = eastNeighbor(_node, root);

	// Check if this leaf has to split
	bool hasToSplit = false;
	if (nNbor && !(nNbor->isALeaf)
		&& (!(nNbor->SW->isALeaf) || !( nNbor->SE->isALeaf))) {
		hasToSplit = true;
	}
	else if (sNbor && !(sNbor->isALeaf)
		&& (!(sNbor->NW->isALeaf) || !(sNbor->NE->isALeaf))) {
		hasToSplit = true;
	}
	else if (wNbor && !(wNbor->isALeaf )
		&& (!(wNbor->NE->isALeaf) || !(wNbor->SE->isALeaf))) {
		hasToSplit = true;
	}
	else if (eNbor && !(eNbor->isALeaf)
		&& (!(eNbor->NW->isALeaf ) || !(eNbor->SW->isALeaf))) {
		hasToSplit = true;
	}

	//if (nNbor && !(nNbor->isALeaf || nNbor->isAEmptyNode)
	//	&& (!(nNbor->SW->isALeaf || nNbor->SW->isAEmptyNode) || !( nNbor->SE->isALeaf || nNbor->SE->isAEmptyNode))) {
	//	hasToSplit = true;
	//}
	//else if (sNbor && !(sNbor->isALeaf || sNbor->isAEmptyNode)
	//	&& (!(sNbor->NW->isALeaf|| sNbor->NW->isAEmptyNode) || !(sNbor->NE->isALeaf || sNbor->NE->isAEmptyNode))) {
	//	hasToSplit = true;
	//}
	//else if (wNbor && !(wNbor->isALeaf || wNbor->isAEmptyNode)
	//	&& (!(wNbor->NE->isALeaf || wNbor->NE->isAEmptyNode) || !(wNbor->SE->isALeaf || wNbor->SE->isAEmptyNode))) {
	//	hasToSplit = true;
	//}
	//else if (eNbor && !(eNbor->isALeaf || eNbor->isAEmptyNode)
	//	&& (!(eNbor->NW->isALeaf || eNbor->NW->isAEmptyNode) || !(eNbor->SW->isALeaf || eNbor->SW->isAEmptyNode))) {
	//	hasToSplit = true;
	//}

	return hasToSplit;
}

void jmk::QuadTree::BalanceTheTree(){
	std::vector<jmk::QDTNode*> leafNodes;
	getLeafNodes(root, leafNodes);

	while (!leafNodes.empty()) {
		auto leaf = leafNodes.back();
		leafNodes.pop_back();

		bool hasToSplit = needToSplit(leaf, root);
		if (hasToSplit) {
			// Split the this node in to four childs.
			auto box = leaf->box;
			float x_mid = (box.x_min + box.x_max) / 2;
			float y_mid = (box.y_min + box.y_max) / 2;

			jmk::AABB boxNW{ box.x_min, x_mid, y_mid, box.y_max };
			jmk::AABB boxNE{ x_mid, box.x_max, y_mid, box.y_max };
			jmk::AABB boxSW{ box.x_min, x_mid, box.y_min, y_mid };
			jmk::AABB boxSE{ x_mid, box.x_max, box.y_min, y_mid };

			// Create 4 childs and set the bounds
			auto nodeNW = new jmk::QDTNode();
			auto nodeNE = new jmk::QDTNode();
			auto nodeSW = new jmk::QDTNode();
			auto nodeSE = new jmk::QDTNode();

			nodeNW->box = boxNW;
			nodeNE->box = boxNE;
			nodeSW->box = boxSW;
			nodeSE->box = boxSE;

			nodeNW->isALeaf = true;
			nodeNE->isALeaf = true;
			nodeSW->isALeaf = true;
			nodeSE->isALeaf = true;
			
			if (boxNW.isInside(leaf->point))
				nodeNW->point = leaf->point;
			else if (boxNE.isInside(leaf->point))
				nodeNE->point = leaf->point;
			else if (boxSW.isInside(leaf->point))
				nodeSW->point = leaf->point;
			else
				nodeSE->point = leaf->point;

			leaf->NW = nodeNW;
			leaf->NE = nodeNE;
			leaf->SW = nodeSW;
			leaf->SE = nodeSE;
			nodeNW->parent = leaf;
			nodeNE->parent = leaf;
			nodeSW->parent = leaf;
			nodeSE->parent = leaf;

			//Leaf is no longer a empty node or a leaf node
			leaf->isALeaf = false;

			leafNodes.push_back(nodeNW);
			leafNodes.push_back(nodeNE);
			leafNodes.push_back(nodeSW);
			leafNodes.push_back(nodeSE);

			// Check if neighbours have to split or not
			auto nNbor = northNeighbor(leaf, root);
			auto sNbor = southNeighbor(leaf, root);
			auto wNbor = westNeighbor(leaf, root);
			auto eNbor = eastNeighbor(leaf, root);

			if( isEndNode(nNbor) && needToSplit(nNbor,root))
				leafNodes.push_back(nNbor);

			if (isEndNode(sNbor) && needToSplit(sNbor, root))
				leafNodes.push_back(sNbor);

			if (isEndNode(eNbor) && needToSplit(eNbor, root))
				leafNodes.push_back(eNbor);

			if (isEndNode(wNbor) && needToSplit(wNbor, root))
				leafNodes.push_back(wNbor);
		}
	}
}

void jmk::QuadTree::GenerateMesh()
{
}

void jmk::QuadTree::GetUniqueSegmentList(std::vector<Segment2d>& _segList){
	if (root) {
		auto box = root->box;
		Point2d bot_left(box.x_min, box.y_min);
		Point2d bot_right(box.x_max, box.y_min);
		Point2d top_left(box.x_min, box.y_max);
		Point2d top_right(box.x_max, box.y_max);
		_segList.push_back(Segment2d(bot_left,  bot_right));
		_segList.push_back(Segment2d(bot_right, top_right));
		_segList.push_back(Segment2d(top_right, top_left));
		_segList.push_back(Segment2d(top_left,  bot_left));

		addBoundariesToTheList(root, _segList);
	}
}
