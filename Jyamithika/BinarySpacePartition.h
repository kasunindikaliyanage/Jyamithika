#pragma once


#include "Core\Base\Core.h"
#include "Core\Base\Vector.h"
#include "Core\Primitives\Line.h"
#include "Core\Primitives\Point.h"
#include "Core\GeoUtils.h"

#include <vector>
#include <algorithm>

namespace jmk {
#define MIN_ELEMENTS_PER_PARTITION  4

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

	class BSP2DLines {
		struct BSP2DLinesNode {
			BSP2DLinesNode* neg = nullptr;
			BSP2DLinesNode* pos = nullptr;
			jmk::Line2dStd split_line;
			std::vector<jmk::Segment2d> segment_list;

			BSP2DLinesNode(std::vector<jmk::Segment2d> _points) : segment_list(_points) {
			}

			BSP2DLinesNode(jmk::Line2dStd _line,
				BSP2DLinesNode* _neg = nullptr, BSP2DLinesNode* _pos = nullptr)
				: split_line(_line), neg(_neg), pos(_pos) {
			}

			BSP2DLinesNode(std::vector<jmk::Segment2d> _points, jmk::Line2dStd _line,
				BSP2DLinesNode* _neg = nullptr, BSP2DLinesNode* _pos = nullptr)
				: segment_list(_points), split_line(_line), neg(_neg), pos(_pos)
			{}
		};


	public:
		BSP2DLines(std::vector<jmk::Segment2d>& _segment_list) {

		}
	};
}