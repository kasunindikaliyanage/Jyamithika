#pragma once

#include "glad\glad.h"

class VertexBuffer {
private :
	unsigned int id =0;
public :

	VertexBuffer();

	VertexBuffer( int count );

	VertexBuffer(const float* data, int count);

	~VertexBuffer();

	void bind();

	unsigned int get_id();
};