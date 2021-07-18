#include "Distance.h"

float jmk::distance(Point3d& A, Point3d& B, Point3d& C)
{
	Vector3f AB = B - A;	//direction vector of line
	Vector3f CA = A - C;
	Vector3f cross = crossProduct(CA, AB);
	
	float mag_cross = cross.magnitude();
	float mag_dir = AB.magnitude();

	float distance = mag_cross / mag_dir;
	return distance;
}

float jmk::distance(Line& line, Point3d& C)
{
	// TODO we can simple call distance frunction which takes three points here as well.
	// But then the methodology of finding the distance is not visible to students. So keep this.
	// Ignor the fact that we duplicate the similar implementation, which is not the best practise.
	Vector3f CA = line.point() - C ;
	Vector3f dir = line.direction();
	Vector3f cross = crossProduct(CA, dir );
	
	float mag_cross = cross.magnitude();
	float mag_dir = dir.magnitude();

	float distance = mag_cross / mag_dir;
	return distance;
}
