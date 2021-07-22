#include "Voronoi.h"

#include <algorithm>
#include <queue>
#include <list>
#include <Iterator>
#include "Core\Primitives\Line.h"
#include "Core\Primitives\Segment.h"
#include "Core\Intersection.h"
#include "Core\Distance.h"

using namespace jmk;

enum class EVENT_TYPE {
	SITE,
	CIRCLE
};

enum class BEACH_ITEM_TYPE {
	ARC,
	EDGE
};

struct Event;

typedef Line2d EdgeItem;

struct BeachLineItem {
	Event* event = nullptr;
	Point2d site;
	BEACH_ITEM_TYPE type;

	BeachLineItem* next_arc = nullptr;
	BeachLineItem* prev_arc = nullptr;
	BeachLineItem* next_edge = nullptr;
	BeachLineItem* prev_edge = nullptr;

	EdgeItem edge;			 // For edge items
};

struct Event {
	EVENT_TYPE type;
	Point2d site;
	Point2d* intersetion_point; // For circle events
	BeachLineItem* arc;
	bool valid = true;
};

struct EventComparator {
	bool operator()(const Event* e1, const Event* e2)
	{
		Point2d point = e1->site;
		Point2d other = e2->site;

		if (point[Y] > other[Y])
			return true;
		else if (isEqualD(point[Y], other[Y]) && point[Y] < other[Y])
		{
			return true;
		}

		return false;
	}
};

std::priority_queue< Event*, std::vector<Event*>,EventComparator> p_queue;

std::vector<Segment> segments;
std::list<BeachLineItem*> beach_line;
typedef std::list<BeachLineItem*>::iterator BeachLineItr;

float computeArcY(Point2d& _arc_site, Point2d& _current_point)
{
	auto yf = _arc_site[Y];
	auto yd = _current_point[Y];
	auto xf = _arc_site[X];
	auto xd = _current_point[X];

	//return the y value of arc at this point
	auto s1 = (xd - xf) * (xd - xf);
	auto d1 = 1 / (2 * (yf - yd));
	auto s2 = yf + yd;
	
	return s1 / d1 + s2 / 2;
}

BeachLineItr getArcAbove(Point2d& _point)
{
	BeachLineItr current_arc_itr = beach_line.begin();
	float current_arc_y = FLT_MAX;
	BeachLineItr itr = current_arc_itr;
	
	while (itr != beach_line.end())
	{
		if ((*itr)->type == BEACH_ITEM_TYPE::ARC)
		{
			auto arc_y = computeArcY((*itr)->site, _point);
			if (arc_y < current_arc_y)
			{
				current_arc_itr = itr;
				current_arc_y = arc_y;
			}
		}
	}

	return current_arc_itr;
}

BeachLineItr addNewArc(BeachLineItr& itr, Point2d& site)
{
	BeachLineItem* arc_to_replace = *itr;

	//afl,el,an,er,afr
	BeachLineItem* left_arc		= new BeachLineItem();
	left_arc->type = BEACH_ITEM_TYPE::ARC;
	left_arc->site = arc_to_replace->site;

	BeachLineItem* new_arc = new BeachLineItem();
	left_arc->type = BEACH_ITEM_TYPE::ARC;
	left_arc->site = site;

	BeachLineItem* right_arc	= new BeachLineItem();
	right_arc->type = BEACH_ITEM_TYPE::ARC;
	right_arc->site = arc_to_replace->site;

	//Start point of the edges is 
	auto arc_y = computeArcY((*itr)->site, site);
	Point2d start_point(site[X],arc_y);

	// Direction vectors for the two edges are prependicular to the vector between the two focuses
	// - focus of new_arc and arc_to_replace
	Vector2f fnfo = arc_to_replace->site - new_arc->site;	 // Direction vector from new site to arc_to_replace->site
	Vector2f dir(fnfo[Y], -fnfo[X]);						 // Prependicular vector to fnfo
	Vector2f neg_dir(dir[X], -dir[Y]);
	EdgeItem l_edge(start_point, dir);
	EdgeItem r_edge(start_point, neg_dir);

	BeachLineItem* left_edge = new BeachLineItem();
	left_edge->type = BEACH_ITEM_TYPE::EDGE;
	left_edge->edge = l_edge;
	
	BeachLineItem* right_edge = new BeachLineItem();
	right_edge->type = BEACH_ITEM_TYPE::EDGE;
	right_edge->edge = r_edge;

	left_arc->prev_arc = arc_to_replace->prev_arc;
	left_arc->next_arc = new_arc;
	right_arc->prev_arc = new_arc;
	right_arc->next_arc = arc_to_replace->next_arc;
	new_arc->prev_arc = left_arc;
	new_arc->next_arc = right_arc;

	left_arc->next_edge = left_edge;
	left_arc->prev_edge = arc_to_replace->prev_edge;
	new_arc->prev_edge = left_edge;
	new_arc->next_edge = right_edge;
	right_arc->prev_edge = right_edge;
	right_arc->next_edge = arc_to_replace->next_edge;

	// Add the new itms in order. Add the last one first, then the second to last and so on 
	// -as list::insert add element before the position.
	beach_line.insert(itr, right_arc);
	beach_line.insert(itr, right_edge);
	beach_line.insert(itr, new_arc);
	beach_line.insert(itr, left_edge);
	beach_line.insert(itr, left_arc);

	// delete the original arc finally. We do it finally here to take care the degenerate case of 
	// having only one arc. If we delete it first then the iterator is going to invalid
	// As we add 5 new elements, we need to delete the element 5 position higher than the initial iterator.

	std::advance(itr, 5);
	beach_line.erase(itr);

	//decrement the iterator 3 position to point to the newly added arc.
	std::advance(itr, -3);
	return itr;
}

void addCircleEvents(BeachLineItem* middle_arc)
{
	// Check 3 arcs sequence ending current arc circle event
	if (middle_arc != nullptr && middle_arc->prev_arc != nullptr)
	{
		// Check the intersection of 2 edges separating above arcs
		BeachLineItem* edg1 = middle_arc->prev_edge;
		BeachLineItem* edg2 = middle_arc->next_edge;
		Point2d* intersection_point = new Point2d();

		bool is_intersect = intersect(edg1->edge, edg2->edge, *intersection_point);

		if (is_intersect)
		{
			//Finding the circle event Y. which is (intersection.y - circle.radius)
			float circle_radius = distance(*intersection_point, middle_arc->site);
			float circle_event_y =  (*intersection_point)[Y] - circle_radius;

			// If there is already a circle event for this arc, check the y coordinates.
			// If already exsisting event's y coordinate is lower we can replace the circle event with the new one
			if (middle_arc->event)
			{
				// Make previous circle event invalid one from queue
				if (middle_arc->event->site[Y] > circle_event_y)
					middle_arc->event->valid = false;
				else
					return; // If the previous circle event's y coordinate is higher we keep that event
			}

			Event* circle_event = new Event();
			circle_event->type = EVENT_TYPE::CIRCLE;
			circle_event->arc = middle_arc;
			circle_event->site = Point2d((*intersection_point)[X], circle_event_y);
			circle_event->intersetion_point = intersection_point;
			p_queue.push(circle_event);

			middle_arc->event = circle_event;
		}
	}
}

void handleSiteEvent(Event* event) 
{
	// In site events new arcs is going to form
	if (beach_line.empty())
	{
		// Beach line is emptry. So add the item(Arc) and return
		BeachLineItem* item = new BeachLineItem();
		item->type = BEACH_ITEM_TYPE::ARC;
		beach_line.push_back(item);
		return;
	}
	else
	{
		// Need to find the Arc directly above this site
		BeachLineItr arc_above_itr	= getArcAbove(event->site);
		BeachLineItr new_arc_itr	= addNewArc(arc_above_itr, event->site);

		addCircleEvents((*new_arc_itr)->prev_arc);
		addCircleEvents((*new_arc_itr)->next_arc);
	}
}

void handleCircleEvent(Event* event, std::vector<Edge2d>& _edges)
{
	// In circle events, exsisting arcs are going to shrink to a point
	// Delete el,an,er related to current circle event
	BeachLineItem* arc_to_remove = event->arc;
	Point2d left_point = arc_to_remove->prev_edge->edge.point();
	Point2d right_point = arc_to_remove->next_edge->edge.point();

	Edge2d edg1(left_point, *event->intersetion_point);
	Edge2d edg2(right_point, *event->intersetion_point);

	// Add two completed edges
	_edges.push_back(edg1);
	_edges.push_back(edg2);

	// Remove the arc and add new edge for arc left and right to circle event arc.
	Point2d* start_point = event->intersetion_point;

	float dir_x = 0.0;
	float dir_y = 0.0;

	BeachLineItem* left_arc = arc_to_remove->prev_arc;
	BeachLineItem* right_arc = arc_to_remove->next_arc;
	
	if ( left_arc && right_arc)
	{
		Vector2f fnfo = right_arc->site - left_arc->site;	 // Direction vector from new site to arc_to_replace->site
		dir_x = fnfo[Y];
		dir_y = -fnfo[X];
	}
	else
	{
		dir_y = -1;
	}

	Vector2f dir(dir_x, dir_y);								// Prependicular vector to fnfo
	EdgeItem edge(*start_point, dir);

	BeachLineItem* new_edge = new BeachLineItem();
	new_edge->type = BEACH_ITEM_TYPE::EDGE;
	new_edge->edge = edge;

	//BeachLineItem* left_arc = arc_to_remove->prev_arc;
	//BeachLineItem* right_arc = arc_to_remove->next_arc;

	if (left_arc)
	{
		left_arc->next_arc = arc_to_remove->next_arc;
		left_arc->next_edge = new_edge;
	}

	if (right_arc)
	{
		right_arc->prev_arc = arc_to_remove->prev_arc;
		right_arc->prev_edge = new_edge;
	}

	// Clean up the memory
	delete arc_to_remove->prev_edge;
	delete arc_to_remove->next_edge;
	delete arc_to_remove;
	
	// Need to check for circle event due to this change in beach line
	// two checks, one as left arc as middle and second as right arc as circle event
	addCircleEvents(left_arc);
	addCircleEvents(right_arc);
}

void jmk::constructVoronoiDiagram_fortunes(std::vector<jmk::Point2d>& _points_list, std::vector<jmk::Edge2d>& _edges)
{
	// We need a unique list of points.
	std::sort(_points_list.begin(), _points_list.end(), sort2DTBLR);
	std::unique(_points_list.begin(), _points_list.end());

	for (Point2d& point : _points_list)
	{
		Event* event = new Event();
		event->site = point;
		p_queue.push(event);
	}

	while (!p_queue.empty())
	{
		Event* event = p_queue.top();
		p_queue.pop();

		if (event->valid)
		{
			if (event->type == EVENT_TYPE::SITE)
				handleSiteEvent(event);
			else
				handleCircleEvent(event, _edges);
		}
	}
}
