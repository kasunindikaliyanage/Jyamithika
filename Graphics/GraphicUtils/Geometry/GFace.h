#pragma once

#include "GraphicUtils\Geometry\GLine.h"
#include "Jyamithika\Core\Primitives\Polyhedron.h"

void getDataFromFaceList(std::vector<jmk::Face*>& faces, std::vector<float>& data)
{
	for (size_t i = 0; i < faces.size(); i++)
	{
		for (size_t j = 0; j < faces[i]->vertices.size(); j++)
		{
			jmk::Point3d ptn = *faces[i]->vertices[j]->point;
			data.push_back(ptn[X]);
			data.push_back(ptn[Y]);
			data.push_back(ptn[Z]);
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