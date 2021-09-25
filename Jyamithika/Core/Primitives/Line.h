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
		Vector2f normal_vec;

	public:

		Line2d() {}

		Line2d( Point2d& p1, Vector2f& _dir )
		{
			point_in_line = p1;
			dir = _dir;
			dir.normalize();
			normal_vec.assign(X, -dir[Y]);
			normal_vec.assign(Y, dir[X]);
		}

		Vector2f direction() const;

		Point2d point() const;

		Vector2f normal() const;
	};

	template<class coord_type, size_t dim = DIM3>
	class LineStd {
		Vector<coord_type, dim> point;
		Vector<coord_type, dim> dir;
		Vector<coord_type, dim> second;
		float d;

	public:
		LineStd() {}

		LineStd(Vector<coord_type, dim>& p1, Vector<coord_type, dim>& p2, bool points = false) {
			if (points) {
				dir = p2 - p1;
				second = p2;
			}
			else
				dir = p2;

			dir.normalize();
			point = p1;
		}

		Vector<coord_type, dim> getPoint() const;

		Vector<coord_type, dim> getSecondPoint() const;

		Vector<coord_type, dim> getDir() const;

		float getD() const;

		void setDirection(Vector<coord_type, dim>& _dir);

		void setPoint(Vector<coord_type, dim>& _point);

		void setD(float value);

	};
	
	template<class coord_type, size_t dim>
	inline Vector<coord_type, dim> LineStd<coord_type, dim>::getPoint()const
	{
		return point;
	}

	template<class coord_type, size_t dim>
	inline Vector<coord_type, dim> LineStd<coord_type, dim>::getSecondPoint() const
	{
		return second;
	}

	template<class coord_type, size_t dim>
	inline Vector<coord_type, dim> LineStd<coord_type, dim>::getDir() const
	{
		return dir;
	}

	template<class coord_type, size_t dim>
	inline float LineStd<coord_type, dim>::getD() const
	{
		return d;
	}

	template<class coord_type, size_t dim>
	inline void LineStd<coord_type, dim>::setDirection(Vector<coord_type, dim>& _dir)
	{
		dir = _dir;
	}

	template<class coord_type, size_t dim>
	inline void LineStd<coord_type, dim>::setPoint(Vector<coord_type, dim>& _point)
	{
		point = _point;
	}

	template<class coord_type, size_t dim>
	inline void LineStd<coord_type, dim>::setD(float value)
	{
		d = value;
	}

	typedef LineStd<float, DIM2> Line2dStd;
	typedef LineStd<float, DIM3> Line3dStd;
}