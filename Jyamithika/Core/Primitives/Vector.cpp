#include "Vector.h"

template<typename coordinate_type, size_t dimensions>
float jmk::dotProduct(Vector<coordinate_type, dimensions>& v1, Vector<coordinate_type, dimensions>& v2)
{
	return 0.0f;
}

template<typename coordinate_type, size_t dimensions>
jmk::Vector<coordinate_type, dimensions> jmk::crossProduct(Vector<coordinate_type, dimensions>& v1, Vector<coordinate_type, dimensions>& v2)
{
	return Vector<coordinate_type, dimensions>();
}