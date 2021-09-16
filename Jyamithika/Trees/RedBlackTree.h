#pragma once

#include <list>
#include <functional>
#include <algorithm>

namespace jmk {
	template<class KType, class VType, class Compare = std::less<KType>>
	class RedBlackTree {
		enum class NodeColor { BLACK, RED };

		struct RBTNode {
			KType key = {};
			VType value = {};
			RBTNode* left = nullptr;
			RBTNode* right = nullptr;
			RBTNode* parent = nullptr;
			NodeColor color = NodeColor::BLACK;

		public:
			RBTNode() {}

			RBTNode(KType& _value, RBTNode* _left = nullptr, RBTNode* _right = nullptr, RBTNode* _parent = nullptr) :
				key(_value), left(_left), right(_right), parent(_parent) {}
		};

		void insert_fixup(RBTNode*);
		void delete_fixup(RBTNode*);
		void transplant(RBTNode*, RBTNode*);
		void rotateLeft(RBTNode*);
		void rotateRight(RBTNode*);

		RBTNode* grandParent(RBTNode*);
		RBTNode* sibling(RBTNode*);
		RBTNode* uncle(RBTNode*);
		RBTNode* distantNephew(RBTNode*);
		RBTNode* closeNephew(RBTNode*);
		void remove(RBTNode*);

		RBTNode* NilNode = new RBTNode();
		RBTNode* root = NilNode;

	public:
		RedBlackTree() {}

		RedBlackTree(std::list<KType>& _data, const unsigned int _root_index = 0) {
			root = new RBTNode(_data[_root_index]);
			for (size_t i = 0; i < _data.size(); i++) {
				if (i != _root_index) {
					insert(_data[i]);
				}
			}
		}

		void insert(const KType& _value);
		void insert(std::list<KType>&);
		void remove(const KType& _value);
		
	};

	template<class KType, class VType, class Compare>
	void RedBlackTree<KType, VType, Compare>::insert_fixup(RBTNode* node){
		RBTNode* current = node;

		while (current->parent->color == NodeColor::RED){
			auto c_uncle = uncle(current);
			auto gparent = grandParent(current);

			if (c_uncle->color == NodeColor::RED) {
				c_uncle->color = NodeColor::BLACK;
				current->parent = NodeColor::BLACK;
				gparent->color = NodeColor::RED;
				current = gparent;
			}
			else {
				if (current == closeNephew(c_uncle)) {
					auto prev_parent = current->parent;
					if (c_uncle == gparent->right)
						rotateLeft(current->parent);
					else
						rotateRight(current->parent);
					current = prev_parent;
				}

				current->parent->color = NodeColor::BLACK;

				gparent->color = NodeColor::RED;
				if (c_uncle == gparent->right)
					rotateLeft(gparent);
				else
					rotateRight(gparent);
			}
		}
		root->color = NodeColor::RED;
	}

	template<class KType, class VType, class Compare>
	void RedBlackTree<KType, VType, Compare>::delete_fixup(RBTNode* node){
		RBTNode* current = node;
		while (current != root && current->color == NodeColor::BLACK) {
			auto w = sibling(current);
			if (w->color == NodeColor::RED) {
				w->color = NodeColor::BLACK;
				x->parent->color = NodeColor::RED;
				if (current == current->parent->left)
					rotateLeft(current->parent);
				else
					rotateRight(current->parent);
				w = sibling(x);
			}

			auto c_nephew = closeNephew(w);
			auto dis_nephew = distantNephew(w);

			if (c_nephew->color == NodeColor::BLACK && dis_nephew->color == NodeColor::BLACK) {
				w->color = NodeColor::RED;
				current = current->parent;
			}
			else {
				if (dis_nephew->color == NodeColor::BLACK) {
					c_nephew->color = NodeColor::BLACK;
					w->color = NodeColor::RED;
					
					if (current == current->parent->left)
						rotateRight(w);
					else
						rotateLeft(w);

					w = sibling(current);
				}

				w->color = current->parent->color;
				current->parent->color = NodeColor::BLACK;
				dis_nephew = distantNephew(current);
				dis_nephew->color = NodeColor::BLACK;

				if (current == current->parent->left)
					rotateLeft(current->parent);
				else
					rotateRight(current->parent);
				current = root;
			}
		}
		current->color = NodeColor::BLACK;
	}

	template<class KType, class VType, class Compare>
	void RedBlackTree<KType, VType, Compare>::transplant(RBTNode* current, RBTNode* replacement){
		if (current->parent == NilNode)
			root = v;
		else if (current == current->parent->left)
			current->parent->left = replacement;
		else
			current->parent->right = replacement;
		replacement->parent = current->parent;
	}

	template<class KType, class VType, class Compare>
	void RedBlackTree<KType, VType, Compare>::rotateLeft(RBTNode*){
	}

	template<class KType, class VType, class Compare>
	void RedBlackTree<KType, VType, Compare>::rotateRight(RBTNode*){
	}

	template<class KType, class VType, class Compare>
	typename RedBlackTree<KType, VType, Compare>::RBTNode* RedBlackTree<KType, VType, Compare>::grandParent(RBTNode* _node){
		if (_node->parent != NilNode)
			return _node->parent->parent;
		return NilNode;
	}

	template<class KType, class VType, class Compare>
	typename RedBlackTree<KType, VType, Compare>::RBTNode* RedBlackTree<KType, VType, Compare>::sibling(RBTNode* _node){
		if (_node->parent != NilNode) {
			if (_node == _node->parent->left)
				return _node->parent->right;
			else
				return _node->parent->left;
		}
		return NilNode;
	}

	template<class KType, class VType, class Compare>
	typename RedBlackTree<KType, VType, Compare>::RBTNode* RedBlackTree<KType, VType, Compare>::uncle(RBTNode* _node){
		if (_node->parent != NilNode)
			return sibling(_node->parent);
		return NilNode;
	}

	template<class KType, class VType, class Compare>
	typename RedBlackTree<KType, VType, Compare>::RBTNode* RedBlackTree<KType, VType, Compare>::distantNephew(RBTNode* _node){
		auto sib = sibling(_node);
		if (sib != NilNode) {
			if (_node == _node->parent->left)
				return sib->right;
			else
				return sib->left;
		}
		return NilNode;
	}

	template<class KType, class VType, class Compare>
	typename RedBlackTree<KType, VType, Compare>::RBTNode* RedBlackTree<KType, VType, Compare>::closeNephew(RBTNode* _node){
		auto sib = sibling(_node);
		if (sib != NilNode) {
			if (_node == _node->parent->left)
				return sib->left;
			else
				return sib->right;
		}
		return NilNode;
	}

	template<class KType, class VType, class Compare>
	void RedBlackTree<KType, VType, Compare>::remove(RBTNode*)
	{
	}

	template<class KType, class VType, class Compare>
	void RedBlackTree<KType, VType, Compare>::insert(const KType& _value) {
		RBTNode* new_node = new RBTNode(_value);

		auto temp_parent = NilNode;
		auto temp = root;

		while (temp != NilNode) {
			temp_parent = temp;
			if (_value < temp->key)
				temp = temp->left;
			else
				temp = temp->right;
		}

		new_node->parent = temp_parent;
		if (temp_parent == NilNode)
			root = new_node;
		else if (_value < temp_parent->key)
			temp_parent->left = new_node;
		else
			temp_parent->right = new_node;

		new_node->left = NilNode;
		new_node->right = NilNode;
		new_node->color = NodeColor::RED;
		insert_fixup(new_node);
	}

	template<class KType, class VType, class Compare>
	void RedBlackTree<KType, VType, Compare>::insert(std::list<KType>&)
	{
		for (const auto& key : _list) {
			insert(key);
		}
	}

	template<class KType, class VType, class Compare>
	void RedBlackTree<KType, VType, Compare>::remove(const KType& _value)
	{
	}
}