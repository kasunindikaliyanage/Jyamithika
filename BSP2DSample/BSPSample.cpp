#include "Jyamithika\Core\Primitives\Point.h"
#include "Jyamithika\BinarySpacePartition.h"
#include "Jyamithika\Triangulation.h"

#include "Jyamithika\BinarySpacePartition.h"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

void setup_segments(std::vector<jmk::Segment2d>& _seg_list){
	//_seg_list.push_back(jmk::Segment2d(jmk::Point2d(1.0,2.5), jmk::Point2d(-1.2,0.2)));
	//_seg_list.push_back(jmk::Segment2d(jmk::Point2d(-4.75, -1.6), jmk::Point2d(-0.4, -2)));
	//_seg_list.push_back(jmk::Segment2d(jmk::Point2d(1.3, 0.75), jmk::Point2d(1, -1.6)));
	//_seg_list.push_back(jmk::Segment2d(jmk::Point2d(2.3, 2.8), jmk::Point2d(4, 1.8)));	
	//_seg_list.push_back(jmk::Segment2d(jmk::Point2d(-1.3,3.2), jmk::Point2d(0.1,3.1)));
	//_seg_list.push_back(jmk::Segment2d(jmk::Point2d(-3.1, 5.4), jmk::Point2d(-2.8, 2.7)));
	//_seg_list.push_back(jmk::Segment2d(jmk::Point2d(-6.1, 5), jmk::Point2d(-4.4, 3.8)));
	//_seg_list.push_back(jmk::Segment2d(jmk::Point2d(-4.5, 2.4), jmk::Point2d(-3.2, 0.15)));
}

int main(){
	std::vector<jmk::Segment2d> segment_list;
	setup_segments(segment_list);

	jmk::BSP2DSegments bspTree(segment_list);
	bspTree.print();

	return 0;
}