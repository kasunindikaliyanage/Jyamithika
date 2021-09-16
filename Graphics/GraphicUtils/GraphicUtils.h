#pragma once

#include <algorithm>
#include <vector>

#include "Jyamithika\Core\Primitives\Point.h"
#include "Jyamithika\Core\Primitives\Polygon.h"
#include "Jyamithika\Core\Primitives\PolygonDCEL.h"

#include "Jyamithika\Trees\KDTree.h"

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

void getReactanglePointClouds(std::list<jmk::Point2d>& positions, std::vector<float>& data);

void getLineDataFromPoints2D(std::list<jmk::Point2d>& positions, std::vector<float>& data);

void get2DLinePointsFromEdgeList(std::vector<jmk::Edge2dSimple>& edges, std::vector<float>&data);

void get2DLinePointsFromFaceEdgeList(std::vector<jmk::Edge2dSimple>& edges, std::vector<float>& data);

void get2DLinePointsFromDCEL2d(std::vector<jmk::EdgeDCEL<float, DIM2>*>& edge_list, std::vector<float>& data);

void getGraphicDataFromPointsList(std::vector<jmk::Point2d>& points, std::vector<float>& data);
