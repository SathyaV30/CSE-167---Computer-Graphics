#include "Curve.h"

// Implementation of the Curve class in Curve.h
// Note that the draw function is virtual, and you will implement 
// suitable extensions in the bezier and bspline classes.
// The other functions just perform management on the point list, and 
// you can likely use those in the skeleton code.
// Rewritten to exclusively use Modern OpenGL by Hoang Tran in August 2016

#ifndef LINE_WIDTH
#define LINE_WIDTH 8.0f
#endif

// This is a simple curve drawer, that just connects the dots for various 
// points (using Pvector iterators).  Draws a curve with line segments. 
// The final step draws the points themselves, in addition to the lines.


// This function connects your control points together. Draws green lines between your curve control points.
void Curve::connectTheDots() {
	Pvector::iterator it, next;
	vector<float> coords;
	// Only connect the points if there is more than 1 point, otherwise there aren't enough points to connect
	if (points.size() > 1) {
		for (it = points.begin(); it != points.end()-1; it++) {
			next = it+1;
			// This is perpendicular to the line connecting x1y1 to x2y2
			float px = next->y - it->y, py = it->x - next->x;
			px /= sqrtf(px * px + py * py); px = px / 2100.0f * LINE_WIDTH;
			py /= sqrtf(px * px + py * py); py = py / 2100.0f * LINE_WIDTH;
			// Since GL_QUADS is straight out removed on some systems (deprecated on other systems),
			// we will draw the lines connecting the points using triangles instead
			coords.push_back(it->x + px);
			coords.push_back(it->y + py);
			coords.push_back(it->x - px);
			coords.push_back(it->y - py);
			coords.push_back(next->x + px);
			coords.push_back(next->y + py);
			coords.push_back(it->x - px);
			coords.push_back(it->y - py);
			coords.push_back(next->x + px);
			coords.push_back(next->y + py);
			coords.push_back(next->x - px);
			coords.push_back(next->y - py);
		}
		// Lines are drawn in green. 2 coordinates make one line, hence why we are drawing using 1/2
		// the size of the vector as the count of indices.
		glUniform3f(colorPos, 0.0, 1.0, 0.0);
		// Send the array of data over to the buffer and get it drawn
		glBufferData(GL_ARRAY_BUFFER, coords.size() * sizeof(float), &coords[0], GL_STATIC_DRAW);
		glDrawArrays(GL_TRIANGLES, 0, coords.size() / 2);
		coords.clear();
	}
	// Now draw the individual points themselves.
	for (it = points.begin(); it != points.end(); it++) {
		it->draw();
		// In case any lines are drawn afterwards, set the color in the shader blue
		glUniform3f(colorPos, 0.0, 0.0, 1.0);
	}
}

// Curve drawing routine.  For the base curve class, it simply calls connect 
// the dots.  However, derived classes like Bezier and Bspline can/will take 
// an additional argument corresponding loosely to level of detail or how 
// many subdivisions/segments to use. 
void Curve::draw(int lod) {
	if (!points.empty()) {
		connectTheDots();
	}
}


// Add a point to the curve.  
// To avoid misclicks, new points are added only if there is no current 
// active point, to avoid ambiguity.  
void Curve::addPoint(float x, float y) {
	if (!isactive) {
		points.push_back(Point(x, y));
	}
}


// Deletes the current active point (hence requires one exists)
// Then resets that there is no active point
void Curve::deleteActivePoint() {
	if (isactive) {
		points.erase(activePoint);
		isactive = false ;
	}
}

// Moves the active point if non-null.
void Curve::moveActivePoint(float dx, float dy) {
	if (isactive) {
		activePoint->x += dx;
		activePoint->y += dy;
	}
}

// Update the active point, when you click on a new one.  
// The first part checks to make sure the click is outside a given 
// radius [i.e. not the same point], and if so, finds the closest point.
// Note that the closest point must be within the radius to be accepted.
void Curve::updateActivePoint(float x, float y) {
	float dx, dy, dist2;
	float radius = Point::ACTIVE_RADIUS*Point::ACTIVE_RADIUS;
	dist2 = 1;
	if (isactive) {
		dx = activePoint->x - x;
		dy = activePoint->y - y;
		dist2 = dx*dx + dy*dy;
	}
	if (dist2 > radius) {
		if (isactive) activePoint->active = false;
		isactive = false ; 
		for (Pvector::iterator it = points.begin(); it != points.end(); it++) {
			dx = it->x - x;
			dy = it->y - y;
			dist2 = dx*dx + dy*dy;
			if (dist2 < radius) {
				activePoint = it;
				it->active = true;
				isactive = true ; 
				break;
			}
		}
	}
}

// Simple auxiliary routine to draw a line between two points.
// Can be called from Bezier/B-Spline curves
void Curve::drawLine(float x1, float y1, float x2, float y2) {
	// Blue color
	glUniform3f(colorPos, 0.0, 0.0, 1.0);
	float px = y2 - y1, py = x1 - x2; // This is perpendicular to the line connecting x1y1 to x2y2
	px /= sqrtf(px * px + py * py); px = px / 2100.0f * LINE_WIDTH;
	py /= sqrtf(px * px + py * py); py = py / 2100.0f * LINE_WIDTH;
	// Since the line width cannot be changed on some systems, we need to draw thick lines as 2 triangles instead.
	float coords[] = { x1 + px, y1 + py, x1 - px, y1 - py, x2 + px, y2 + py,
	x1 - px, y1 - py, x2 + px, y2 + py, x2 - px, y2 - py};
	// Send the vertex data over to the buffer and get it drawn
	glBufferData(GL_ARRAY_BUFFER, sizeof(coords), &coords[0], GL_STATIC_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

// Helper function used by Curve and its subclasses so it can set its color when being drawn.
void Curve::setShaderReferences(GLuint color) {
	colorPos = color;
}

Curve::Curve()
{
	isactive = false ;
}

Curve::~Curve()
{

}

