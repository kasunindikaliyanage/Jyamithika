#pragma once
// Include functions to calculate distance between different geometric promitives
#include "Primitives\Point.h"

namespace jmk {

	/// <summary>
	/// Returns the distance to the point C from line [AB]. Return value > 0 
	/// Not the sign distance.</summary>
	/// <param name="A"></param>
	/// <param name="B"></param>
	/// <param name="C"></param>
	/// <returns></returns>
	float distance(Point3d& A, Point3d& B, Point3d& C);
}