#pragma once

#include <algorithm>
#include <vector>

#include "Jyamithika\Core\Primitives\Point.h"
#include "Jyamithika\Core\Primitives\Polygon.h"

//float Rect2dPoints[] = {
//	-0.008f, -0.008f,  // left  
//	 0.008f, -0.008f,  // right 
//	 0.008f,  0.008f,  // top 
//	 0.008f,  0.008f,  // left  
//	-0.008f, -0.008f,  // right 
//	-0.008f,  0.008f  // top   
//};

std::vector<float> getTranslated2DRectanglePointVertices(jmk::Point2d&);

void getReactanglePointClouds(std::vector<jmk::Point2d>& positions, std::vector<float>& data);

void get2DLinePointsFromEdgeList(std::vector<jmk::Edge2d>& edges, std::vector<float>&data);

void get2DLinePointsFromFaceEdgeList(std::vector<jmk::Edge2d>& edges, std::vector<float>& data);