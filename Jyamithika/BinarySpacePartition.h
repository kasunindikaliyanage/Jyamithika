#pragma once


#include "Core\Base\Core.h"
#include "Core\Base\Vector.h"
#include "Core\Primitives\Line.h"
#include "Core\Primitives\Point.h"
#include "Core\GeoUtils.h"
#include "Core\Intersection.h"

#include <vector>
#include <algorithm>
#include <iostream>

namespace jmk {
#define MIN_ELEMENTS_PER_PARTITION  4

	enum class SEG_TYPES {
		POSITIVE,
		NEGATIVE,
		INTERSECT
	};

	//TODO : Remove this class asap
	class BSP2D {
		struct BSP2DNode {
			BSP2DNode* neg = nullptr;
			BSP2DNode* pos = nullptr;
			jmk::Line2dStd split_line;
			std::vector<jmk::Point2d> points_list;

			BSP2DNode(std::vector<jmk::Point2d> _points) : points_list(_points) {
			}

			BSP2DNode(jmk::Line2dStd _line,
				BSP2DNode* _neg = nullptr, BSP2DNode* _pos = nullptr)
				: split_line(_line), neg(_neg), pos(_pos) {
			}

			BSP2DNode(std::vector<jmk::Point2d> _points, jmk::Line2dStd _line,
				BSP2DNode* _neg = nullptr, BSP2DNode* _pos = nullptr)
				: points_list(_points), split_line(_line), neg(_neg), pos(_pos)
			{}
		};

		BSP2DNode* root = nullptr;

		jmk::Line2dStd getSplitLine(std::vector<jmk::Point2d>& _points_list, jmk::Line2dStd* _prev_line);

		bool isALeaf(BSP2DNode* _node);

		BSP2DNode* constructBSP2D(std::vector<jmk::Point2d>& _points_list, jmk::Line2dStd* _prev_line);

		void getSplitLineList(BSP2DNode* _node,std::vector<jmk::Line2dStd>& _points_list);

	public:
		BSP2D(std::vector<jmk::Point2d>& _points_list) {
			root = constructBSP2D(_points_list, nullptr);
		}

		void getSplitLines(std::vector<jmk::Line2dStd>& _lines_list);
	};

	bool BSP2D::isALeaf(BSP2DNode* _node)
	{
		if (_node && (_node->neg || _node->pos))
			return false;
		return true;
	}

	jmk::Line2dStd BSP2D::getSplitLine(std::vector<jmk::Point2d>& _points_list, jmk::Line2dStd* _prev_line)
	{
		const uint32_t size = _points_list.size();
		std::sort(_points_list.begin(), _points_list.end());
		jmk::Point2d p1, p2;
		jmk::Line2dStd line;

		if (!_prev_line){
			p1.assign(X, (_points_list[0][X] + _points_list[1][X]) / 2);
			p1.assign(Y, (_points_list[0][Y] + _points_list[1][Y]) / 2);
			p2.assign(X, (_points_list[size - 1][X] + _points_list[size - 2][X]) / 2);
			p2.assign(Y, (_points_list[size - 1][Y] + _points_list[size - 2][Y]) / 2);
			line = jmk::Line2dStd(p1, p2, true);
		}
		else if (false){

			float nxy = 0.0, x_sum = 0.0, y_sum = 0.0, n_x_sqr_sum = 0.0;
			
			for (const auto& point : _points_list)
			{
				nxy += point[X] * point[Y];
				x_sum += point[X];
				y_sum += point[Y];
				n_x_sqr_sum += x_sum * x_sum;
			}

			nxy *= size;
			n_x_sqr_sum *= size;

			auto slope = (nxy - (x_sum) * (y_sum)) / (n_x_sqr_sum - x_sum * x_sum);
			auto intercept = (size * y_sum - slope * x_sum) / size;
			
			srand((unsigned)time(0));
			auto s = rand() % 50;
			s /= 25.0;
			p2.assign(X, -slope*s);
			p2.assign(Y, 1+s);

			line = jmk::Line2dStd(p1, p2, false);
		}
		else {
			float x_sum = 0.0, y_sum = 0.0;
			for (const auto& point : _points_list)
			{
				x_sum += point[X];
				y_sum += point[Y];
			}
			p1.assign(X, x_sum/size);
			p1.assign(Y, y_sum/size);

			p2.assign(X, -_prev_line->getDir()[Y]+0.2);
			p2.assign(Y, _prev_line->getDir()[X]+0.2);

			line = jmk::Line2dStd(p1, p2, false);
		}

		Vector2f normal(-line.getDir()[Y], line.getDir()[X]);
		auto d = dotProduct(normal, p2);
		line.setD(d);
		return line;
	}

	BSP2D::BSP2DNode* BSP2D::constructBSP2D(std::vector<jmk::Point2d>& _points_list, 
		jmk::Line2dStd* _prev_line)
	{
		const uint32_t size = _points_list.size();
		if (size <= MIN_ELEMENTS_PER_PARTITION) {

			return new BSP2DNode(_points_list);
		}
		jmk::Line2dStd split_line = getSplitLine(_points_list, _prev_line);

		std::vector<jmk::Point2d> pos_vec;
		std::vector<jmk::Point2d> neg_vec;

		std::copy_if(_points_list.begin(), _points_list.end(),
			std::back_inserter(neg_vec),
			[&](const jmk::Point2d& point)
		{
			return jmk::left(split_line, point);
		});

		std::copy_if(_points_list.begin(), _points_list.end(),
			std::back_inserter(pos_vec),
			[&](const jmk::Point2d& point)
		{
			return !jmk::left(split_line, point);
		});

		auto left = constructBSP2D(neg_vec, &split_line);
		auto right = constructBSP2D(pos_vec, &split_line);

		return new BSP2DNode(split_line, left, right);
	}

	void BSP2D::getSplitLineList(BSP2DNode* _node, std::vector<jmk::Line2dStd>& _lines_list)
	{
		if (!isALeaf(_node)){
			_lines_list.push_back(_node->split_line);
			getSplitLineList(_node->neg, _lines_list);
			getSplitLineList(_node->pos, _lines_list);
		}
	}

	void BSP2D::getSplitLines(std::vector<jmk::Line2dStd>& _lines_list){
		getSplitLineList(root, _lines_list);
	}

	class BSP2DSegments {
		struct BSP2DSegNode {
			BSP2DSegNode* neg = nullptr;
			BSP2DSegNode* pos = nullptr;
			jmk::Line2d split_line;
			jmk::Segment2d segment;

			BSP2DSegNode(jmk::Segment2d _segment) : segment(_segment) {
			}

			BSP2DSegNode(jmk::Line2d _line,
				BSP2DSegNode* _neg = nullptr, BSP2DSegNode* _pos = nullptr)
				: split_line(_line), neg(_neg), pos(_pos) {
			}

			BSP2DSegNode(jmk::Segment2d _segment, jmk::Line2d _line,
				BSP2DSegNode* _neg = nullptr, BSP2DSegNode* _pos = nullptr)
				: segment(_segment), split_line(_line), neg(_neg), pos(_pos)
			{}
		};

		BSP2DSegNode* constructBSP2D(std::vector<jmk::Segment2d>& _seg_list);
		jmk::Line2d getSplitLine(std::vector<jmk::Segment2d>& _seg_list, int& _index);
		SEG_TYPES classifySegmenetToLine(jmk::Segment2d& _seg, jmk::Line2d& _line, jmk::Segment2d& _pos_seg, 
			jmk::Segment2d& _neg_seg);

		BSP2DSegNode* root;

		void printNode(BSP2DSegNode*, int depth);

	public:
		BSP2DSegments(std::vector<jmk::Segment2d>& _segment_list) {
			root = constructBSP2D(_segment_list);
		}

		void print();
	};

	BSP2DSegments::BSP2DSegNode* jmk::BSP2DSegments::constructBSP2D(std::vector<jmk::Segment2d>& _seg_list)
	{
		int size = _seg_list.size();
		if (size > 1) {
			int split_seg_index = -1;
			std::vector<jmk::Segment2d> pos_list, neg_list;
			jmk::Segment2d pos_seg, neg_seg;
			jmk::Line2d split_line = getSplitLine(_seg_list, split_seg_index);

			for (size_t i = 0; i < size; i++){
				if (i != split_seg_index){
					switch (classifySegmenetToLine(_seg_list[i], split_line, pos_seg, neg_seg))
					{
					case SEG_TYPES::INTERSECT :
						pos_list.push_back(pos_seg);
						neg_list.push_back(neg_seg);
						break;
					case SEG_TYPES::POSITIVE:
						pos_list.push_back(pos_seg);
						break;
					default:
						neg_list.push_back(pos_seg);
						break;
					}
				}
			}

			BSP2DSegNode* left = constructBSP2D(pos_list);
			BSP2DSegNode* right = constructBSP2D(neg_list);
			return new BSP2DSegNode(_seg_list[split_seg_index],split_line, left, right);
		}
	}
	
	inline jmk::Line2d BSP2DSegments::getSplitLine(std::vector<jmk::Segment2d>& _seg_list, int& _index)
	{
		int min_intersections = INT_MAX;
		Line2d min_intersect_line;

		for (size_t i = 0; i < _seg_list.size(); i++){
			auto seg = _seg_list[i];
			Vector2f dir = seg.p2 - seg.p1;
			Line2d line(seg.p1, dir);
			int this_intersections = 0;
			
			for (size_t j = 0; j < _seg_list.size(); j++){
				if(i != j)
					if(jmk::intersect(line, _seg_list[j]))
						this_intersections++;
			}

			if (this_intersections < min_intersections) {
				min_intersect_line = line;
				_index = i;
			}
		}
		return min_intersect_line;
	}

	inline SEG_TYPES BSP2DSegments::classifySegmenetToLine(jmk::Segment2d& _seg, jmk::Line2d& _line,
		jmk::Segment2d& _pos_seg, jmk::Segment2d& _neg_seg)
	{
		jmk::Point2d intersect_point;
		bool is_intersect = jmk::intersect(_line, _seg, intersect_point);
		if (is_intersect) {
			if (left(_line, _seg.p1)) {
				_pos_seg.p1 = _seg.p1;
				_pos_seg.p2 = intersect_point;
				_neg_seg.p1 = intersect_point;
				_neg_seg.p2 = _seg.p2;
			}
			else {
				_pos_seg.p1 = _seg.p2;
				_pos_seg.p2 = intersect_point;
				_neg_seg.p1 = intersect_point;
				_neg_seg.p2 = _seg.p1;
			}
			return SEG_TYPES::INTERSECT;
		}
		else if(left(_line, _seg.p1)) {
			return SEG_TYPES::POSITIVE;
		}

		return SEG_TYPES::NEGATIVE;
	}

	void BSP2DSegments::printNode(BSP2DSegNode* _node, int depth) {
		if (!_node)
			return;

		printNode(_node->pos, depth+1);
		
		int space = 0;
		while (space < depth)
			std::cout << " ";
		std::cout << "< (" << _node->segment.p1[X] << "," << _node->segment.p1[Y] << ") - ("
			<< _node->segment.p2[X] << "," << _node->segment.p2[Y] << ")\n";
		
		printNode(_node->neg, depth + 1);
	}

	void BSP2DSegments::print() {
		printNode(root, 0);
	}
}