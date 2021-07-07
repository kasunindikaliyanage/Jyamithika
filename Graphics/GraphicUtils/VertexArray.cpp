#include "VertexArray.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &id);
	glBindVertexArray(id);
}

void VertexArray::addVertexLayout(int _index, int _numberOfElements, bool _isNormalized, int _stride_bytes, int _first_offset_bytes)
{
	glVertexAttribPointer(_index, _numberOfElements, GL_FLOAT, _isNormalized, _stride_bytes, (void*)_first_offset_bytes);
	glEnableVertexAttribArray(_index);
}

void VertexArray::bindVertexArray()
{
	glBindVertexArray(id);
}

unsigned int VertexArray::get_id()
{
	return id;
}

VertexArray::~VertexArray()
{
	glDeleteBuffers(1, &id);
}