#pragma once
// Include functions to calculate intersection between different geometric promitives
#include "Primitives\Point.h"
#include "Primitives\Line.h"
#include "Primitives\Plane.h"
#include "Primitives\Polygon.h"
#include "Primitives\Polyhedron.h"

bool intersect( const jmk::Line&, const jmk::Line&, jmk::Point3d&);

bool intersect( const jmk::Line2d&, const jmk::Line2d&, jmk::Point2d&);