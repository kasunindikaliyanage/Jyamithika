#pragma once
#include "../Base/Vector.h"
#include "Point.h"

namespace jmk {
	
	template<class T>
	class Plane {
		// Based on the normal-point form ( n . X = d ) definition of a plane 
		Vector3f normal;
		float d = 0;
	public:

		Plane() {}

		Plane(Vector3f& _normal) :normal(_normal) {
			normal.normalize();
		}

		// Make sure to calculate the _constant using normalized vector.
		Plane(Vector3f& _normal, float _constant) : normal(_normal), d(_constant) {
			normal.normalize();
		}

		Plane(Vector3f& _normal, Point3d& _point)
		{
			normal = _normal;
			d = dotProduct(normal, _point);
		}

		Plane(Point3d& _p1, Point3d& _p2, Point3d& _p3)
		{
			Vector3f v21 = _p2 - _p1;
			Vector3f v31 = _p3 - _p1;

			normal = crossProduct3d(v21, v31);
			normal.normalize();

			d = dotProduct(normal, _p1);
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

		Vector<T> getNormal() const
		{
			return normal;
		}

		float getD() const
		{
			return d;
		}
	};

	typedef Plane<float> Planef;
}