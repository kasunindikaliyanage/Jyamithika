#pragma once
// TODO Implement the BST, Braided Search tree and Randomized Search tree as well

#include <list>
#include <functional>
#include <algorithm>

#include "TreeNodes.h"

namespace jmk {

	template< class KType, class VType, class Compare = std::less<KType>>
	class BST {
	
		struct BSTNode {
			KType key = {};
			VType value = {};
			BSTNode* left = nullptr;
			BSTNode* right = nullptr;
			BSTNode* parent = nullptr;

		public:
			BSTNode() {}

			BSTNode(KType& _key, VType& _value , BSTNode* _left = nullptr, BSTNode* _right = nullptr, BSTNode* _parent = nullptr) :
				key(_key), value(_value), left(_left), right(_right), parent(_parent) {}
		};

		BSTNode* root = nullptr;
		Compare comp;

		BSTNode* find(KType& _value);
		void transplant(BSTNode* found, BSTNode* replacement);
		BSTNode* treeMin(BSTNode* branch_root);
		BSTNode* treeMax(BSTNode* branch_root);
		BSTNode* findSplitNode(KType& _min, KType& _max);
		bool isLeaf(BSTNode* _node);
		void addTreeToAList(BSTNode* _node, std::list<VType>&, bool addFront = false);

	public:
		BST(KType& _key, VType& _value) {
			root = new BSTNode(_key,_value);
		}

		BST(std::list<std::pair<KType,VType>>& _data, const unsigned int _root_index = 0) {
			root = new BSTNode(_data[_root_index].first, _data[_root_index].second);
			for (size_t i = 0; i < _data.size(); i++) {
				if (i != _root_index) {
					insert(_data[i]);
				}
			}
		}

		void insert( const KType& _value);
		void insert( std::list<KType>&);
		void remove( const KType& _value);
		void find( const KType&, const KType&, std::list<VType>&);
		std::pair<KType, VType> predecessor(const KType& _value);
		std::pair<KType, VType> successor(const KType& _value);

		std::pair<KType, VType> minimum(BSTNode* _node = nullptr);
		std::pair<KType, VType> maximum(BSTNode* _node = nullptr);
		bool isEmpty() {
			if (root)
				return true;
			return false;
		}

		void inOrderTravers(BSTNode*,std::list<VType>&);
		void preOrderTravers(BSTNode*, std::list<VType>&);
		void postOrderTravers(BSTNode*, std::list<VType>&);
	};

	template<class KType, class VType, class Compare>
	typename BST<KType, VType, Compare>::BSTNode* BST<KType, VType, Compare>::find(KType& _key){
		BSTNode* current = root;
		while (current && current->key != _key){
			if (comp(current->key, _key))
				current = current->right_child;
			else
				current = current->left_child;
		}

		// Given key is not found in this tree
		return current;
	}

	template<class KType, class VType, class Compare>
	void BST<KType, VType, Compare>::transplant(BSTNode* u, BSTNode* v)
	{
		if (!u->parent)
			root = v;
		else if (u == u->parent->left_child)
			u->parent->left_child = v;
		else
			u->parent->right_child = v;

		if (v)
			v->parent = u->parent;
	}

	template<class KType, class VType, class Compare>
	typename BST<KType, VType, Compare>::BSTNode* BST<KType, VType, Compare>::treeMin(BSTNode* _branch_root)
	{
		BSTNode* temp_node = _branch_root;
		while (temp_node && temp_node->left_child){
			temp_node = temp_node->left_child;
		}
		temp_node;
	}

	template<class KType, class VType, class Compare>
	typename BST<KType, VType, Compare>::BSTNode* BST<KType, VType, Compare>::treeMax(BSTNode* _branch_root){
		BSTNode* temp_node = _branch_root;
		while (temp_node && temp_node->right_child){
			temp_node = temp_node->right_child;
		}

		return temp_node;
	}

	template<class KType, class VType, class Compare>
	typename BST<KType, VType, Compare>::BSTNode* BST<KType, VType, Compare>::findSplitNode(KType& _min, KType& _max){
		auto v = root;
		while (!isLeaf(v) && ( _max <= v->key || _min > v->key)) {
			if (_max <= v->key)
				v = v->left;
			else
				v = v->right;
		}
		return v;
	}

	template<class KType, class VType, class Compare>
	bool BST<KType, VType, Compare>::isLeaf(BSTNode* _node)
	{
		if (_node && _node->left && _node->right)
			return true;
		return false;
	}

	template<class KType, class VType, class Compare>
	void BST<KType, VType, Compare>::addTreeToAList(BSTNode* _node, std::list<VType>& _list, bool addFront){
		std::list<VType> values;
		inOrderTravers(_node, values);
		if(addFront)
			_list.insert(_list.begin(), values.begin(), values.end());
		else
			_list.insert(_list.end(), values.begin(), values.end());
	}

	template<class KType, class VType, class Compare>
	void BST<KType, VType, Compare>::insert(const KType& _value){	
		if (!root) {
			root = new BSTNode(_value);
			return;
		}
		else{
			BSTNode* temp = root;
			while (true) {
				if (comp(temp->key, _value)) {
					if (temp->left)
						temp = temp->left;
					else {
						temp->left = new BSTNode(_value);
						temp->left->parent = temp;
						break;
					}
				}
				else {
					if (temp->right)
						temp = temp->right;
					else {
						temp->right = new BSTNode(_value);
						temp->right->parent = temp;
						break;
					}
				}
			}
		}
	}

	template<class KType, class VType, class Compare>
	void BST<KType, VType, Compare>::insert(std::list<KType>& _list){
		for (const auto& key : _list ){
			insert(key);
		}
	}
	
	template<class KType, class VType, class Compare>
	void BST<KType, VType, Compare>::remove(const KType& _value){
		BSTNode* current_node = find(_value);

		if (current_node){
			BSTNode* current_left = current_node->left_child;
			BSTNode* current_right = current_node->right_child;

			if (isLeaf(current_node))
				transplant(current_node, nullptr)

			if (!current_left){
				transplant(current_node, current_right);
			}
			else if (!current_right){
				transplant(current_node, current_left);
			}
			else{
				BSTNode* right_min = treeMin(current_right);
				if (right_min->parent != current_node){
					transplant(right_min, right_min->right_child);
					right_min->right_child = current_node->right_child;
					right_min->right_child->parent = right_min;
				}

				transplant(current_node, right_min);
				right_min->left_child = current_node->left_child;
				right_min->left_child->parent = right_min;
			}
		}
	}

	template<class KType, class VType, class Compare>
	void BST<KType, VType, Compare>::find(const KType& _min, const KType& _max, std::list<VType>& _list)
	{
		auto v_split = findSplitNode(_min, _max);
		if (isLeaf(v_split)) {
			if (v->key >= _min && v->key < _max)
				_list.insert(_list.push_front(v->key));
		}
		else {
			//Follow the path to left boundary
			auto v = v_split->left;
			while (!isLeaf(v)) {
				if (_min <= v->key) {
					addTreeToAList(v->right, _list, true);
					_list.push_front(v->key);
					v = v->left;
				}
				else
					v = v->right;
			}
			if (v->key >= _min)
				_list.insert(_list.push_front(v->key));

			v = v_split->right;
			while (!isLeaf(v)) {
				if (_max >= v->key) {
					addTreeToAList(v->left, _list);
					v = v->right;
				}
				else
					v = v->left;
			}
			if (v->key <= _max)
				_list.insert(_list.push_back(v->key));
		}
	}

	template<class KType, class VType, class Compare>
	std::pair<KType, VType> BST<KType, VType, Compare>::predecessor(const KType& _value) {
		//BSTNode* current_node = find(_value);
		//if (current_node){
		//	BSTNode* max_left = nullptr;
		//	max_left = treeMax(current_node->left_child);
		//	if (max_left)
		//		return max_left->key;
		//	else {
		//		auto parent = current_node->parent;
		//		while (parent && parent->parent){
		//			parent = parent->parent;
		//			if (parent->left)
		//				return parent->key;
		//		}
		//	}
		//}
		//return _value;
	}

	template<class KType, class VType, class Compare>
	std::pair<KType, VType> BST<KType, VType, Compare>::successor(const KType& _value) {
		//BSTNode* current_node = find(_value);
		//if (current_node){
		//	BSTNode* min_right = nullptr;
		//	min_right = treeMax(current_node->right_child);
		//	if (min_right)
		//		return min_right->key;
		//	else {
		//		auto parent = current_node->parent;
		//		while (parent && parent->parent) {
		//			parent = parent->parent;
		//			if (parent->right)
		//				return parent->key;
		//		}
		//	}
		//}
		//return _value;
	}
	
	template<class KType, class VType, class Compare>
	void BST<KType, VType, Compare>::inOrderTravers(BSTNode* _node, std::list<VType>& _list){	
		if (!_node)
			return;
		inOrderTravers(_node->left, _list);
		_list.push_back(_node->value);
		inOrderTravers(_node->right, _list);
	}
	
	template<class KType, class VType, class Compare>
	void BST<KType, VType, Compare>::preOrderTravers(BSTNode* _node, std::list<VType>& _list){
		if (!_node)
			return;
		_list.push_back(_node->value);
		inOrderTravers(_node->left, _list);
		inOrderTravers(_node->right, _list);
	}
	
	template<class KType, class VType, class Compare>
	void BST<KType, VType, Compare>::postOrderTravers(BSTNode* _node, std::list<VType>& _list){
		if (!_node)
			return;
		inOrderTravers(_node->left, _list);
		inOrderTravers(_node->right, _list);
		_list.push_back(_node->value);
	}\

	template<class KType, class VType, class Compare>
	std::pair<KType, VType> BST<KType, VType, Compare>::minimum(BSTNode* _node){
		if (!_node)
			_node = root;
		auto temp = _node;
		while (temp->left)
			temp = temp->left;
		return std::pair<KType, VType>(temp->key, temp->value);
	}

	template<class KType, class VType, class Compare>
	std::pair<KType, VType> BST<KType, VType, Compare>::maximum(BSTNode* _node){
		if (!_node)
			_node = root;
		auto temp = _node;
		while (temp->right)
			temp = temp->right;
		return std::pair<KType, VType>(temp->key, temp->value);
	}
}