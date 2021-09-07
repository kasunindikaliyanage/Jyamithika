#pragma once
#include "../Base/Vector.h"
#include "Point.h"

namespace jmk {
	class Line {
		Vector3f dir;				// Normalized direction vector. 
		Point3d point_in_line;

	public:

		Line() {}

		Line(Point3d& p1, Point3d& p2)
		{
			point_in_line = p1;
			dir = p2 - p1;
			dir.normalize();
		}

		void setDirection(Vector3f&);

		void setPoint(Point3d&);

		Vector3f direction() const;

		Point3d point() const;
	};

	class Line2d {
		Vector2f dir;
		Point2d point_in_line;

	public:

		Line2d() {}

		Line2d( Point2d& p1, Vector2f& _dir )
		{
			point_in_line = p1;
			dir = _dir;
			dir.normalize();
		}

		Vector2f direction() const;

		Point2d point() const;
	};


	//template<class coord_types, size_t dimension = DIM3>
	//class Line {
	//	Vector<coord_types, dimension> point;
	//	Vector<coord_types, dimension> dir;

	//public:
	//	Line() {}

	//	Line(Vector<coord_types, dimension>& p1, Vector<coord_types, dimension>& p2)
	//	{
	//		dir = p2 - p1;
	//		point = p1;
	//	}


	//};
}