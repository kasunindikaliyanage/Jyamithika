#include "Vector.h"

//template<typename coordinate_type, size_t dimensions>
//float jmk::dotProduct(Vector<coordinate_type, dimensions>& v1, Vector<coordinate_type, dimensions>& v2)
//{
//	if (v1.coords.size() != v2.coords.size())
//		return FLT_MIN;
//
//	float product = 0;
//	for (int i = 0; i < v1.coords.size(); i++)
//		product = product + v1[i] * v2[i];
//	return product;
//}

jmk::Vector3f jmk::crossProduct(Vector3f a, Vector3f b)
{
	float x_, y_, z_;
	x_ = a[Y] * b[Z] - b[Y] * a[Z];
	y_ = a[Z] * b[X] - b[Z] * a[X];
	z_ = a[X] * b[Y] - b[X] * a[Y];

	return Vector3f(x_, y_, z_);
}

float jmk::scalerTripleProduct(Vector3f a, Vector3f b, Vector3f c)
{
	Vector3f b_cross_c = crossProduct(b, c);
	float value = dotProduct(a, b_cross_c);
	return value;
}

bool jmk::orthogonal(Vector3f a, Vector3f b)
{
	float value = dotProduct(a, b);
	return isEqualD(value, 0.0);
}

jmk::Vector2f jmk::prependicluar(Vector2f& vec)
{
	return Vector2f( vec[Y], -vec[X]);
}
