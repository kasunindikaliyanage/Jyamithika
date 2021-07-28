#include "GraphicUtils.h"

#define NUM_POINT_2D_REACT 12

float Rect2dPoints[] = {
	-0.01f, -0.01f,  // left  
	 0.01f, -0.01f,  // right 
	 0.01f,  0.01f,  // top 
	 0.01f,  0.01f,  // left  
	-0.01f, -0.01f,  // right 
	-0.01f,  0.01f  // top   
};

std::vector<float> getTranslated2DRectanglePointVertices(jmk::Point2d& _point)
{
	std::vector<float> data;
	for (size_t i = 0; i < NUM_POINT_2D_REACT; i++)
	{
		if(i%2 == 0)
			data.push_back(Rect2dPoints[i]+_point[X]);
		else
			data.push_back(Rect2dPoints[i] + _point[Y]);
	}
	return data;
}

void getReactanglePointClouds(std::vector<jmk::Point2d>& positions, std::vector<float>& data)
{
	for (auto point : positions)
	{
		std::vector<float> point_data = getTranslated2DRectanglePointVertices(point);
		data.insert(data.end(), point_data.begin(), point_data.end());
	}
}

void get2DLinePointsFromEdgeList(std::vector<jmk::Edge2d>& edges, std::vector<float>& data)
{
	for (const auto& edge : edges)
	{
		data.push_back(edge.p1[X]);
		data.push_back(edge.p1[Y]);
		data.push_back(edge.p2[X]);
		data.push_back(edge.p2[Y]);
	}
}


void get2DLinePointsFromFaceEdgeList(std::vector<jmk::Edge2d>& edges, std::vector<float>& data)
{
	for (const auto& edge : edges)
	{
		if (edge.fp1 != jmk::DEFAULT_POINT_2D && edge.fp2 != jmk::DEFAULT_POINT_2D)
		{
			data.push_back(edge.fp1[X]);
			data.push_back(edge.fp1[Y]);
			data.push_back(edge.fp2[X]);
			data.push_back(edge.fp2[Y]);
		}
	}
}
