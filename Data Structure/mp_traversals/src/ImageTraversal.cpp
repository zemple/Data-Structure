#include <cmath>
#include <iterator>
#include <iostream>

#include "cs225/HSLAPixel.h"
#include "cs225/PNG.h"
#include "Point.h"

#include "ImageTraversal.h"
#include <iostream>

namespace Traversals {
  /**
  * Calculates a metric for the difference between two pixels, used to
  * calculate if a pixel is within a tolerance.
  *
  * @param p1 First pixel
  * @param p2 Second pixel
  * @return the difference between two HSLAPixels
  */
  double calculateDelta(const HSLAPixel & p1, const HSLAPixel & p2) {
    double h = fabs(p1.h - p2.h);
    double s = p1.s - p2.s;
    double l = p1.l - p2.l;

    // Handle the case where we found the bigger angle between two hues:
    if (h > 180) { h = 360 - h; }
    h /= 360;
    return sqrt( (h*h) + (s*s) + (l*l) );
  }
  
  /**
  * Adds a Point for the bfs traversal to visit at some point in the future.
  * @param work_list the structure which stores the list of points which need to be visited by the traversal
  * @param point the point to be added
  */
  void bfs_add(std::deque<Point> & work_list, const Point & point) {
    /** @todo [Part 1] */
    work_list.push_front(point);
	  isBfs = true;
  } 

  /**
  * Adds a Point for the dfs traversal to visit at some point in the future.
  * @param work_list the structure which stores the list of points which need to be visited by the traversal
  * @param point the point to be added
  */
  void dfs_add(std::deque<Point> & work_list, const Point & point) {
    /** @todo [Part 1] */
    work_list.push_front(point);
	  isBfs = false;
  }

  /**
  * Removes and returns the current Point in the bfs traversal
  * @param work_list the structure which stores the list of points which need to be visited by the traversal
  */
  Point bfs_pop(std::deque<Point> & work_list) {
    /** @todo [Part 1] */
    Point p = work_list.back();
    work_list.pop_back();
    return p;
  }

  /**
  * Removes and returns the current Point in the dfs traversal
  * @param work_list the structure which stores the list of points which need to be visited by the traversal
  */
  Point dfs_pop(std::deque<Point> & work_list) {
    /** @todo [Part 1] */
    Point p = work_list.front();
    work_list.pop_front();
    return p;
  }
  
  Point ImageTraversal::peek(std::deque<Point> & work_list) {
	  if (isBfs) {
      return work_list.back();
    }
	  return work_list.front();
  }

  /**
  * Initializes a ImageTraversal on a given `png` image,
  * starting at `start`, and with a given `tolerance`.
  * @param png The image this traversal is going to traverse
  * @param start The start point of this traversal
  * @param tolerance If the current point is too different (difference larger than tolerance) with the start point,
  * it will not be included in this traversal
  * @param traversal_add a function pointer to an implemented function which takes in a reference to the work list
  * and the point to be added
  * @param traversal_pop a function pointer to an implemented function which takes in a reference to the work list
  * and returns the next point to be processed in the traversal
  */
  ImageTraversal::ImageTraversal(const PNG & png, const Point & start, double tolerance, add_function traversal_add, pop_function traversal_pop) :
	it(this, png, start, tolerance, traversal_add, traversal_pop) {  
    /** @todo [Part 1] */
	  this->start = start;
	  this->png = png;
	  this->tolerance = tolerance;
  }

  /**
  * Returns an iterator for the traversal starting at the first point.
  */
  ImageTraversal::Iterator ImageTraversal::begin() {
    /** @todo [Part 1] */
    return ImageTraversal::Iterator(this, png, start, tolerance, add, pop);
  }

  /**
  * Returns an iterator for the traversal one past the end of the traversal.
  */
  ImageTraversal::Iterator ImageTraversal::end() {
    /** @todo [Part 1] */
	  return ImageTraversal::Iterator();
  }

  /**
  * Default iterator constructor.
  */
  ImageTraversal::Iterator::Iterator() {
    /** @todo [Part 1] */
	  img = NULL;
  }


  ImageTraversal::Iterator::Iterator(ImageTraversal* img, const PNG & png, const Point & start, double tolerance, add_function traversal_add, pop_function traversal_pop) {
	  this -> img = img;
	  base = png;
	  top = start;
	  position = start;
	  toler = tolerance;
	  unsigned int width = 0;
	  unsigned int height = 0;
	  unsigned int aw = base.width();
	  unsigned int ah = base.height();
	
	  img->add = traversal_add;
	  img->pop = traversal_pop;
	
	  visited.resize(aw, std::vector<bool>(ah));
	  while(width < aw) {
		  height = 0;
		  while(height < ah) {
		    visited[width][height] = false;
		    height++;
	  	}
		  width++;
	  }
	  if(isAllowed(position)) {
		  visited[position.x][position.y] = true;
	  }
	  img->add(work_list, start);
  }
  

  /**
  * Iterator increment opreator.
  *
  * Advances the traversal of the image.
  */
  ImageTraversal::Iterator & ImageTraversal::Iterator::operator++() {
    /** @todo [Part 1] */
	  visited[position.x][position.y] = true;
	  Point right, left, above, below;
	  right = Point(position.x + 1, position.y);
	  left = Point(position.x - 1, position.y);
	  above = Point(position.x, position.y - 1);
	  below = Point(position.x, position.y + 1);

	  if(isAllowed(right)) { 
		  img -> add(work_list, right);
	  }
	  if(isAllowed(below)){
		  img -> add(work_list, below);
	  }
	  if(isAllowed(left)) {
		  img -> add(work_list, left);
	  }
	  if(isAllowed(above)) {
		  img -> add(work_list, above);
	  }
	  
	  while(!work_list.empty() && !isAllowed(img -> peek(work_list))) {
		  img -> pop(work_list);
		  if(work_list.empty()) {
			  return *this;
		  }
	  }

	  if(!work_list.empty()) {
		  position = img -> peek(work_list);
		  return *this;
  	} else {
		  img = NULL;
		  return *this;
	  }
	  return *this;
  }

  /**
  * Iterator accessor opreator.
  *
  * Accesses the current Point in the ImageTraversal.
  */
  Point ImageTraversal::Iterator::operator*() {
    /** @todo [Part 1] */
    return position;
  }

  /**
  * Iterator inequality operator.
  *
  * Determines if two iterators are not equal.
  */
  bool ImageTraversal::Iterator::operator!=(const ImageTraversal::Iterator &other) {
    /** @todo [Part 1] */
	  bool thisEmpty = false;
	  bool otherEmpty = false;

	  if (img == NULL) {
		  thisEmpty = true; 
	  }
	  if (other.img == NULL) {
		  otherEmpty = true; 
	  }
	  if (!thisEmpty)  { 
		  thisEmpty = work_list.empty(); 
	  }
	  if (!otherEmpty) { 
		  otherEmpty = other.work_list.empty(); 
	  }
	  if (thisEmpty && otherEmpty) {
      return false; 
    } else if ((!thisEmpty) && (!otherEmpty)) {
      return (img != other.img); 
    }
	  return true; 
  }
  
  bool ImageTraversal::Iterator::isAllowed(Point p) {
	  if(p.y >= base.height() || p.x >= base.width()) {
      return false;
    }
	  HSLAPixel& begin = base.getPixel(top.x, top.y);
	  HSLAPixel& position = base.getPixel(p.x, p.y);
	  if(calculateDelta(begin, position) < toler) {
	    if(!visited[p.x][p.y]) {
		    return true;
	    }
	  }
	  return false;
  }

}