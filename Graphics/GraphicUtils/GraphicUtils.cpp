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

float colorsnew[36] = {
	0.95 , 0.45 , 0.2,  // Orangish
	0.2 ,  0.95 , 0.4,  // Greenish
	0.95 , 0.2 ,  0.84,	// Pickish
	1.0 , 0.0 , 0.0,	// Red
	0.0 , 1.0 , 0.0,	// Green
	0.0 , 0.0 , 1.0,	// Blue
	0.6 ,  0.2 , 0.95,	// Violetish
	0.2 ,  0.94 , 0.95, // Cyan
	0.9 , 0.95, 0.2,	// Yellow
	0.6 , 0.95 ,0.2,	// light Green
	0.8,  0.1 , 0.2, //11
	0.0 , 0.0 , 1.2	 //12
};

static int current_color_index = 0;


std::vector<float> getTranslated2DRectanglePointVertices(jmk::Point2d& _point)
{
	std::vector<float> data;
	for (size_t i = 0; i < NUM_POINT_2D_REACT; i++)
	{
		if(i%2 == 0)
			data.push_back(Rect2dPoints[i] +_point[X]);
		else
			data.push_back(Rect2dPoints[i] + _point[Y]);
	}
	return data;
}

void getReactanglePointClouds(std::vector<jmk::Point2d>& positions, std::vector<float>& data){
	for (auto point : positions){
		std::vector<float> point_data = getTranslated2DRectanglePointVertices(point);
		data.insert(data.end(), point_data.begin(), point_data.end());
	}
}

void getReactanglePointClouds(std::list<jmk::Point2d>& positions, std::vector<float>& data){
	for (auto point : positions){
		std::vector<float> point_data = getTranslated2DRectanglePointVertices(point);
		data.insert(data.end(), point_data.begin(), point_data.end());
	}
}

void getLineDataFromPoints2D(std::list<jmk::Point2d>& positions, std::vector<float>& data)
{
	for (const auto& point : positions){
		data.push_back(point[X]);
		data.push_back(point[Y]);
	}
}

void get2DLinePointsFromEdgeList(std::vector<jmk::Edge2dSimple>& edges, std::vector<float>& data)
{
	for (const auto& edge : edges)
	{
		data.push_back(edge.p1[X]);
		data.push_back(edge.p1[Y]);
		data.push_back(edge.p2[X]);
		data.push_back(edge.p2[Y]);
	}
}

void get2DLinePointsFromSegmentList(std::vector<jmk::Segment2d>& edges, std::vector<float>& data){
	for (const auto& edge : edges){
		if (edge.p1 != jmk::DEFAULT_POINT_2D && edge.p2 != jmk::DEFAULT_POINT_2D){
			data.push_back(edge.p1[X]);
			data.push_back(edge.p1[Y]);
			data.push_back(edge.p2[X]);
			data.push_back(edge.p2[Y]);
		}
	}
}

void get2DLinePointsFromFaceEdgeList(std::vector<jmk::Edge2dSimple>& edges, std::vector<float>& data)
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

void get2DLinePointsFromDCEL2d(std::vector<jmk::EdgeDCEL<float, DIM2>*>& edges, std::vector<float>& data)
{
	for (const auto& edge : edges)
	{
		auto origin = edge->origin->point;
		auto dest = edge->destination()->point;
		data.push_back(origin[X]);
		data.push_back(origin[Y]);
		data.push_back(dest[X]);
		data.push_back(dest[Y]);
	}
}

void getGraphicDataFromPointsList(std::vector<jmk::Point2d>& points, std::vector<float>& data)
{
	for (const auto& point : points)
	{
		data.push_back(point[X]);
		data.push_back(point[Y]);
		data.push_back(0.0f);

		//Push colors
		data.push_back(colorsnew[current_color_index % 6 * 3 + 0]);
		data.push_back(colorsnew[current_color_index % 6 * 3 + 1]);
		data.push_back(colorsnew[current_color_index % 6 * 3 + 2]);
	}
	current_color_index++;
}



void get2DLineData(jmk::Line2dStd line, std::vector<float>& data)
{
	auto p1 = line.getPoint();
	auto dir = line.getDir();
	auto p2 = line.getSecondPoint();

	auto x1 = (dir[X] / dir[Y]) * (10 - p1[Y]) + p1[X];
	auto x2 = (dir[X] / dir[Y]) * (-10 - p1[Y]) + p1[X];

	data.push_back(x1);
	data.push_back(10);	
	data.push_back(x2);
	data.push_back(-10);
}

void get2DDataFromLinesList(std::vector<jmk::Line2dStd>& lines, std::vector<float>& data)
{
	for (const auto& line : lines) {
		get2DLineData(line, data);
	}
}



