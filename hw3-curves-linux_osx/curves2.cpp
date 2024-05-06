#include "WorkingScene.h"

// This file includes the basic functions that your program must fill in.  
// Your assignment consists of filling in parts that say /* YOUR CODE HERE */
// There are many helpful functions that can be found in Curve.cpp! In particular,
// take a look at moveActivePoint, drawLine, connectTheDots, etc.

// What happens when you drag the mouse to x and y?  
// In essence, you are dragging control points on the curve.
void WorkingScene::drag(int x, int y) {
	/* YOUR CODE HERE */
	//you must figure out how to transform x and y so they make sense
	//update oldx, and oldy
	//make sure scene gets redrawn
	theOnlyCurve->moveActivePoint((x - oldx) / (1.0f * width), 
                                  (oldy - y) / (1.0f * height));
    oldx = x;
    oldy = y;
	glutPostRedisplay();

	
}

// Mouse motion.  You need to respond to left clicks (to add points on curve) 
// and right clicks (to delete points on curve) 
void WorkingScene::mouse(int button, int state, int x, int y) {

	float unitX = (1.0f * x) / width;
	float unitY = 1.0f - (1.0f * y) / height;
		if (theOnlyCurve && state == GLUT_DOWN) {

		if (button == GLUT_LEFT_BUTTON) {
			/* YOUR CODE HERE */
			theOnlyCurve->addPoint(unitX, unitY);
		}
		if (button == GLUT_RIGHT_BUTTON) {	
			/* YOUR CODE HERE */
			theOnlyCurve->updateActivePoint(unitX, unitY);
			theOnlyCurve->deleteActivePoint();

		}
	}

	/* YOUR CODE HERE */
	//update oldx, and oldy
	//make sure scene gets redrawn
	oldx = x;
	oldy = y;
	glutPostRedisplay();
}



#include "Bezier.h"

//helper function for calculating a single point on the bezier curve
Point calculateBezierPoint(const Pvector& points, float u, int start, int end) {
    if (start == end) {
        return points[start];
    }
    Point p1 = calculateBezierPoint(points, u, start, end - 1);
    Point p2 = calculateBezierPoint(points, u, start + 1, end);
    float x = (1.0f - u) * p1.x + u * p2.x;
    float y = (1.0f - u) * p1.y + u * p2.y;
    return Point(x, y);
}

// Bezier drawing function.  This is by deCasteljau or equivalent algorithm. 
// It should support Bezier curves of arbitrary degree/order.
void Bezier::draw(int levelOfDetail) {
    connectTheDots();
    if (points.size() < 2 || levelOfDetail < 1) return; 

    Pvector curvePoints;

    for (int j = 0; j <= levelOfDetail; j++) {
        float u = j / (1.0f * levelOfDetail);
        Point pt = calculateBezierPoint(points, u, 0, points.size() - 1);
        curvePoints.push_back(pt);
    }

    for (int i = 1; i < curvePoints.size(); i++) {
        drawLine(curvePoints[i - 1].x, curvePoints[i - 1].y, curvePoints[i].x, curvePoints[i].y);
    }
}


#include "Bspline.h"

//helper function for calculating a single point on the bspline curve
Point calculateBSplinePoint(const Pvector::iterator& p1, const Pvector::iterator& p2, 
                            const Pvector::iterator& p3, const Pvector::iterator& p4, 
                            float u) {
    float b0 = (1 - u) * (1 - u) * (1 - u) / 6.0f;
    float b1 = (3 * u * u * u - 6 * u * u + 4) / 6.0f;
    float b2 = (-3 * u * u * u + 3 * u * u + 3 * u + 1) / 6.0f;
    float b3 = u * u * u / 6.0f;

    float x = b0 * p1->x + b1 * p2->x + b2 * p3->x + b3 * p4->x;
    float y = b0 * p1->y + b1 * p2->y + b2 * p3->y + b3 * p4->y;

    return Point(x, y);
}

// The B-Spline drawing routine.  
// Remember to call drawSegment (auxiliary function) for each set of 4 points.
void Bspline::draw(int levelOfDetail) {
  connectTheDots();
  if (points.size() < 4) return; 
  for (int i = 0; i <= points.size() - 4; i++) {
    drawSegment(points.begin() + i, points.begin() + i + 1, points.begin() + i + 2, points.begin() + i + 3, levelOfDetail);
  }
}


void Bspline::drawSegment(Pvector::iterator p1, Pvector::iterator p2, 
                          Pvector::iterator p3, Pvector::iterator p4, int levelOfDetail) {

	/* YOUR CODE HERE */
    if (levelOfDetail <= 0) return; 

    Point oldPoint = calculateBSplinePoint(p1, p2, p3, p4, 0.0f); 
    oldPoint.draw();
	Point newPoint = Point(0.0f,0.0f);
    for (int i = 1; i <= levelOfDetail; i++) {
        float u = i / (1.0f * levelOfDetail); 
        newPoint = calculateBSplinePoint(p1, p2, p3, p4, u);
        drawLine(oldPoint.x, oldPoint.y, newPoint.x, newPoint.y);
        oldPoint = newPoint; 
    }
	Point p(newPoint.x, newPoint.y);
	p.draw();


}


#include "Bezier2.h"

//This function is provided to aid you.
//It should be used in the spirit of recursion, though you may choose not to.
//This function takes an empty vector of points, accum
//It also takes a set of control points, pts, and fills accum with
//the control points that correspond to the next level of detail.
void accumulateNextLevel(Pvector* accum, Pvector pts) {
	if (pts.empty()) return; 
	accum->push_back(*(pts.begin()));
	if (pts.size() == 1) return;
	for (Pvector::iterator it = pts.begin(); it != pts.end() - 1; it++) {
		/* YOUR CODE HERE  (only one to three lines)*/
		 it->x = 0.5f * (it->x + (it+1)->x);
		 it->y = 0.5f * (it->y + (it+1)->y);

	}
	//save the last point
	Point last = *(pts.end()-1);
	pts.pop_back();
	//recursive call
	accumulateNextLevel(accum, pts);
	accum->push_back(last);
}


// The basic draw function for Bezier2.  Note that as opposed to Bezier, 
// this draws the curve by recursive subdivision.  So, levelofdetail 
// corresponds to how many times to recurse.  
void Bezier2::draw(int levelOfDetail) {
	//This is just a trick to find out if this is the top level call
	//All recursive calls will be given a negative integer, to be flipped here
	if (levelOfDetail > 0) {
		connectTheDots();
	} else {
		levelOfDetail = -levelOfDetail;
	}

	//Base case.  No more recursive calls.
	if (levelOfDetail <= 1) {
		if (points.size() >= 2) {
			for (Pvector::iterator it = points.begin(); it != points.end() - 1; it++) {
				/* YOUR CODE HERE */
				drawLine(it->x, it->y, (it + 1)->x, (it + 1)->y);

			}
		}
	} else {
		Pvector* accum = new Pvector();
		accumulateNextLevel(accum, points);
		Bezier2 left, right;

		//add the correct points to 'left' and 'right'.
		//You may or may not use accum as you see fit.
		/* YOUR CODE HERE */
		Pvector::iterator it = accum->begin();
		int size = accum->size();
		bool isOdd = size % 2 == 1;
		int midpoint = size / 2; 
	
		for (int i = 0; i < midpoint + isOdd; i++) {
			left.points.push_back(*it);
			it++;
		}
		it = accum->begin() + midpoint;
		for (int i = midpoint; i < size; i++) {
			right.points.push_back(*it);
			it++;
		}
		left.draw(1 - levelOfDetail);
		right.draw(1 - levelOfDetail);
		delete accum;


	}
}
