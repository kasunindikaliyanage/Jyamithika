#pragma once
// Include functions to calculate intersection between different geometric promitives
#include "Primitives\Point.h"
#include "Primitives\Line.h"
#include "Primitives\Plane.h"
#include "Primitives\Polygon.h"
#include "Primitives\Polyhedron.h"

namespace jmk {

	bool intersect(jmk::Line&, jmk::Line&, jmk::Point3d&);

	bool intersect(jmk::Line2d&,jmk::Line2d&, jmk::Point2d&);
}