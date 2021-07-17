#pragma once

#include "GraphicUtils\Geometry\GLine.h"
#include "Jyamithika\Core\Primitives\Polyhedron.h"

float colors[36] = {
	0.1 , 0.2 , 0.3, //1
	0.6 , 0.3 , 0.4, //2
	0.3 , 0.4 , 0.5, //3
	0.4 , 0.5 , 0.6, //4
	0.5 , 0.6 , 0.7, //5
	0.6 , 0.7 , 0.8, //6
	0.7 , 0.8 , 0.9, //7
	0.8 , 0.9 , 0.1, //8
	0.9 , 0.1 , 0.2, //9
	0.5 , 0.2 , 0.3, //10
	0.8 , 0.7 , 0.3, //11
	0.9 , 0.9 , 0.3	 //12
};

void getDataFromFaceList(std::vector<jmk::Face*>& faces, std::vector<float>& data)
{

	for (size_t i = 0; i < faces.size(); i++)
	{
		if (!faces[i]->normal_switch_needed)
		{
			for (size_t j = 0; j < faces[i]->vertices.size(); j++)
			{
				jmk::Point3d ptn = *faces[i]->vertices[j]->point;
				data.push_back(ptn[X]);
				data.push_back(ptn[Y]);
				data.push_back(ptn[Z]);

				//Push colors
				data.push_back(colors[i * 3 + 0]);
				data.push_back(colors[i * 3 + 1]);
				data.push_back(colors[i * 3 + 2]);
			}
		}
		else
		{
			for ( int j = (faces[i]->vertices.size() - 1) ; j >=0 ; j--)
			{
				jmk::Point3d ptn = *faces[i]->vertices[j]->point;
				data.push_back(ptn[X]);
				data.push_back(ptn[Y]);
				data.push_back(ptn[Z]);

				//Push colors
				data.push_back(colors[i * 3 + 0]);
				data.push_back(colors[i * 3 + 1]);
				data.push_back(colors[i * 3 + 2] );
			}
		}

	}
}


void getLinesFromAFace(jmk::Face* _f, std::vector<GLine*>& _lines)
{
	int size = _f->vertices.size();
	for (size_t j = 0; j < size; j++)
	{
		jmk::Point3d ptn1 = *_f->vertices[j]->point;
		jmk::Point3d ptn2 = *_f->vertices[(j+1)% size]->point;
		
		_lines.push_back(new GLine(glm::vec3(ptn1[X], ptn1[Y], ptn1[Z]), glm::vec3(ptn2[X], ptn2[Y], ptn2[Z])));
	}
}