#include "VertexBuffer.h"

VertexBuffer::VertexBuffer()
{
	glGenBuffers(1, &id);
}

VertexBuffer::VertexBuffer(int count)
{
	glGenBuffers( 1, &id );
	glBindBuffer( GL_ARRAY_BUFFER, id );
	glBufferData( GL_ARRAY_BUFFER, count * sizeof(float), nullptr, GL_DYNAMIC_DRAW );
}

VertexBuffer::VertexBuffer(const float* data, int count)
{
	glGenBuffers(1, &id);
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), data, GL_STATIC_DRAW);
}

unsigned int VertexBuffer::get_id()
{
	return id;
}

void VertexBuffer::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, id);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &id);
}