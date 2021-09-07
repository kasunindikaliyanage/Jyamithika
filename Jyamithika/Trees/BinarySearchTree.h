#pragma once
// TODO Implement the BST, Braided Search tree and Randomized Search tree as well

#include <list>
#include <functional>
#include <algorithm>

#include "TreeNodes.h"

namespace jmk {

	template< class T, class Compare = std::less<T>>
	class BST {
		
		struct BSTNode {
			T value = {};
			BSTNode* left = nullptr;
			BSTNode* right = nullptr;
			BSTNode* parent = nullptr;

		public:
			BSTNode() {}

			BSTNode(T& _value, BSTNode* _left = nullptr, BSTNode* _right = nullptr, BSTNode* _parent = nullptr) :
				value(_value), left(_left), right(_right), parent(_parent) {}
		};

		BSTNode* root = nullptr;
		Compare comp;

		BSTNode* find(T& _value);
		void transplant(BSTNode* found, BSTNode* replacement);
		BSTNode* treeMin(BSTNode* branch_root);
		BSTNode* treeMax(BSTNode* branch_root);

	public:
		BST(T& _value) {
			root = new BSTNode(_value);
		}

		BST(std::list<T>& _data, const unsigned int _root_index = 0) {
			root = new BSTNode(_data[_root_index]);
			for (size_t i = 0; i < _data.size(); i++) {
				if (i != _root_index) {
					insert(_data[i]);
				}
			}
		}

		void insert( const T& _value);
		void remove( const T& _value);
		void find( const T&, const T&, std::list<T>&);
		T left(const T& _value);
		T right(const T& _value);
	};

	template<class T, class Compare>
	BST<T, Compare>::BSTNode* BST<T, Compare>::find(T& _value)
	{
		BSTNode* current = root;
		while (current){
			if (current->value == _value)
				return current;

			if (comp(current->value, _value))
				current = current->right_child;
			else
				current = current->left_child;
		}

		// Given key is not found in this tree
		return nullptr;
	}

	template<class T, class Compare>
	void BST<T, Compare>::transplant(BSTNode* node, BSTNode* replacement)
	{
		if (!node->parent)
			root = replacement;
		else if (node == node->parent->left_child)
			node->parent->left_child = replacement;
		else
			node->parent->right_child = replacement;

		if (replacement)
			replacement->parent = node->parent;

		delete node;
	}

	template<class T, class Compare>
	BST<T, Compare>::BSTNode* BST<T, Compare>::treeMin(BSTNode* branch_root)
	{
		BSTNode* temp_node = _branch_root;
		while (temp_node && temp_node->left_child){
			temp_node = temp_node->left_child;
		}
		temp_node;
	}

	template<class T, class Compare>
	BST<T, Compare>::BSTNode* BST<T, Compare>::treeMax(BSTNode* branch_root){
		BSTNode<T>* temp_node = _branch_root;
		while (temp_node && temp_node->right_child){
			temp_node = temp_node->right_child;
		}

		return temp_node;
	}

	template<class T, class Compare>
	void BST<T, Compare>::insert(const T& _value){	
		if (!root) {
			root = new BSTNode(_value);
		}
		else{
			BSTNode* temp = root;
			while (true) {
				if (comp(temp->value, _value)) {
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
	
	template<class T, class Compare>
	void BST<T, Compare>::remove(const T& _value){
		BSTNode* current_node = find(_value);

		if (current_node){
			BSTNode* current_left = current_node->left_child;
			BSTNode* current_right = current_node->right_child;

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

	template<class T, class Compare>
	void BST<T, Compare>::find(const T& _min, const T& _max, std::list<T>& _list)
	{


	}

	template<class T, class Compare>
	T BST<T, Compare>::left(const T& _value) {
		BSTNode* current_node = find(_value);
		if (current_node){
			BSTNode* max_left = nullptr;
			max_left = treeMax(current_node->left_child);
			if (max_left)
				return max_left->value;
			else {
				auto parent = current_node->parent;
				while (parent && parent->parent){
					parent = parent->parent;
					if (parent->left)
						return parent->value;
				}
			}
		}
		return _value;
	}

	template<class T, class Compare>
	T BST<T, Compare>::right(const T& _value) {
		BSTNode* current_node = find(_value);
		if (current_node){
			BSTNode<T>* min_right = nullptr;
			min_right = treeMax(current_node->right_child);
			if (min_right)
				return min_right->value;
			else {
				auto parent = current_node->parent;
				while (parent && parent->parent) {
					parent = parent->parent;
					if (parent->right)
						return parent->value;
				}
			}
		}
		return _value;
	}
}