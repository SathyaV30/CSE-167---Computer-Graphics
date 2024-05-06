#if !defined(CURVE_H)
#define CURVE_H

#include <vector>
// OSX systems require its own headers
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif
#include "Point.h"
#include <math.h>

// To account for C++ namespace issues
#define vector std::vector  
typedef vector<Point> Pvector;

// The basic curve class.  The basic element is a vector of Points, 
// which is the Pvector.  
// The class has methods for drawing adding, deleting, moving, updating, 
// and so forth.  More details are in Curve.cpp 
// The draw method is virtual since it is over-ridden by the specific 
// curve (Bspline, Bezier, Bezier2 etc.).  

// points is the actual vector of points used.  
// The iterator into this (look up C++ iterators) is activePoint 
// The bool isactive indicates if there is currently any active point 
// (In previous versions, we did this by setting activePoint to null, but 
// this is no longer allowed in modern C++)


class Curve  
{
	protected:
		Pvector points;
		Pvector::iterator activePoint;
		bool isactive ;
		GLuint colorPos;

	public:
		Curve();
		virtual ~Curve();
		virtual void draw(int levelOfDetail);
		void addPoint(float x, float y);
		void deleteActivePoint();
		void moveActivePoint(float dx, float dy);
		void updateActivePoint(float x, float y);
		void connectTheDots();
		void drawLine(float x1, float y1, float x2, float y2) ;
		void setShaderReferences(GLuint);
};

#endif

