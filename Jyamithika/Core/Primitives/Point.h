#pragma once

#include "Vector.h"

namespace jmk {
	class Point3D 
	{
		Vector3f point;

	public:
		Vector3f getV()
		{
			return point;
		}
	};

	class Point2D 
	{
		Vector2f point;
	};
}