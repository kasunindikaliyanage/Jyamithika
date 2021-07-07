#pragma once

#include "glad\glad.h"
#include <vector>

class VertexArray {
private:
	unsigned int id;
public:
	VertexArray();

	void addVertexLayout(int _index, int _numberOfElements, bool _isNormalized, int _stride_bytes, int _first_offset_bytes);

	void bindVertexArray();

	unsigned int get_id();

	~VertexArray();
};