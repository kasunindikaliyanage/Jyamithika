#pragma once
#include "Vector.h"
#include "Point.h"

namespace jmk {
	
	template<class T>
	class Plane {
		// Based on the normal-point form ( n . X = d ) definition of a plane 
		Vector3f normal;
		float d = 0;
	public:

		Plane() {}

		Plane(Vector3f& _normal) :normal(_normal) {}

		Plane(Vector3f& _normal, float _constant) : normal(_normal), d(_constant) {}

		Plane(Point3D& _p1, Point3D& _p2, Point3D& _p3)
		{
			Vector3f v1 = _p1.getV();
			Vector3f v2 = _p2.getV();
			Vector3f v3 = _p3.getV();

			Vector3f v21 = v2 - v1;
			Vector3f v31 = v3 - v1;

			normal = crossProduct(v21, v31);

			d = dotProduct(normal, v1);
		}

		//Eqality check
		bool operator==(const Plane<T>& _other)
		{
			if (normal == _other.normal && isEqualD(d, _other.d))
			{
				return true;
			}
			return false;
		}

		//Not equal check operator
		bool operator!=(const Plane<T>& _other)
		{
			return !(*this == _other);
		}

		Vector<T> getNormal()
		{
			return normal;
		}

		float getD()
		{
			return d;
		}
	};
}