/**************************************************************************/
/* The Point class draws a control point for a curve, along with a circle */
/* Rewritten in August 2016 by Hoang Tran to only use modern OpenGL       */
/**************************************************************************/

#include <vector>
//OSX systems require their own headers
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif
#include <math.h>
#include "Point.h"
#include "Scene.h"

const float Point::ACTIVE_RADIUS = 0.05;

#ifndef LINE_WIDTH
#define LINE_WIDTH 8.0f
#endif

// Implements the methods in Point.h 
// The draw technique 
// For active points, this is just a circle to highlight the point. 
// In any event, a small box around the point.  

void Point::draw() {
	vector<float> coords;
	if (active) {
		// Red color circle
		glUniform3f(colorPos, 1.0, 0.0, 0.0);
		float u,v;
		float nextu, nextv;
		float px, py;
		// Cannot draw using GL_QUADS or GL_LINES on some systems, so we will need
		// to draw the circle around the point using small triangles instead.
		for (float t = 0; t < 2*3.14159; t+= 0.1) {
			u = cos(t)*ACTIVE_RADIUS + x;
			v = sin(t)*ACTIVE_RADIUS + y;
			nextu = cos(t + 0.1)*ACTIVE_RADIUS + x;
			nextv = sin(t + 0.1)*ACTIVE_RADIUS + y;
			px = nextv - v; py = u - nextu;
			px /= sqrtf(px * px + py * py); px = px / 2100.0f * LINE_WIDTH;
			py /= sqrtf(px * px + py * py); py = py / 2100.0f * LINE_WIDTH;
			coords.push_back(u + px);
			coords.push_back(v + py);
			coords.push_back(u - px);
			coords.push_back(v - py);
			coords.push_back(nextu + px);
			coords.push_back(nextv + py);
			coords.push_back(u - px);
			coords.push_back(v - py);
			coords.push_back(nextu + px);
			coords.push_back(nextv + py);
			coords.push_back(nextu - px);
			coords.push_back(nextv - py);
		}
		// Send the array of data over to the buffer and draw the circle
		glBufferData(GL_ARRAY_BUFFER, coords.size() * sizeof(float), &coords[0], GL_STATIC_DRAW);
		glDrawArrays(GL_TRIANGLES, 0, coords.size() / 2);
	}
	// White color
	glUniform3f(colorPos, 1.0, 1.0, 1.0);
	coords.clear();
	// Now draw the point itself. Can't draw a quad on some systems so we draw 2 triangles.
	coords.push_back(x - 0.01);
	coords.push_back(y - 0.01);
	coords.push_back(x + 0.01);
	coords.push_back(y - 0.01);
	coords.push_back(x - 0.01);
	coords.push_back(y + 0.01);
	coords.push_back(x + 0.01);
	coords.push_back(y - 0.01);
	coords.push_back(x - 0.01);
	coords.push_back(y + 0.01);
	coords.push_back(x + 0.01);
	coords.push_back(y + 0.01);
	// Send this data over to the buffer and draw it
	glBufferData(GL_ARRAY_BUFFER, coords.size() * sizeof(float), &coords[0], GL_STATIC_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, coords.size() / 2);
	coords.clear();
}

Point::Point(float xpos, float ypos)
{
	x = xpos;
	y = ypos;
	active = false;
	// Get the position of the one needed uniform variable from Scene so the color can be properly set in draw()
	colorPos = Scene::colorPos;
}

Point::~Point()
{

}

