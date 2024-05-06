#include "Scene.h"
#include "Bezier.h"
#include "Bspline.h"
#include "Bezier2.h"
#include <cstdio>
#include <cstdlib>

// This is the basic class that handles scene setup.  It implements the 
// functions in Scene.h  


// Setting up some important default parameters.  
// The height and width are simply height and width of the window. 
// Oldx and Oldy will be used for scene manipulation, currently 0 
// (example for mouse movements, dragging etc.)
// levelofdetail is used for Bezier and Bspline and defaults to 5.

int Scene::height = 800;
int Scene::width = 800;
int Scene::oldx = 0;
int Scene::oldy = 0;
int Scene::levelOfDetail = 5;

// Needed for the shaders
GLuint Scene::colorPos;

// Initially, we don't have a curve.  That will be set once one one clicks 
// on one of the numeric keys to set the curve type.  
Curve * Scene::theOnlyCurve = NULL;

// Pressing 0, 1, 2, 3 sets the type of curve (basic curve straight line, 
// Bezier, Bspline or Bezier2 [Bezier2 is by subdivision]).  
// + and - alter level of detail, q quits. 
void Scene::keyboard( unsigned char key, int x, int y )
{
	switch(key) {
		case '0':
			if (theOnlyCurve) delete theOnlyCurve;
			theOnlyCurve = new Curve();
			theOnlyCurve->setShaderReferences(colorPos);
			printf("Straight Line.\n");
			break;
		case '1':
			if (theOnlyCurve) delete theOnlyCurve;
			theOnlyCurve = new Bezier();
			theOnlyCurve->setShaderReferences(colorPos);
			printf("Bezier Curve.\n");
			break;
		case '2':
			if (theOnlyCurve) delete theOnlyCurve;
			theOnlyCurve = new Bspline();
			theOnlyCurve->setShaderReferences(colorPos);
			printf("Bspline Curve.\n");
			break;
		case '3':
			if (theOnlyCurve) delete theOnlyCurve;
			theOnlyCurve = new Bezier2();
			theOnlyCurve->setShaderReferences(colorPos);
			printf("Bezier 2 Curve.\n");
			break;
		case '+':
			levelOfDetail++;
			break;
		case '-':
			levelOfDetail--;
			break;
		case 'q':
		case 27:
			exit(0) ;
			break ;
	}
	if (levelOfDetail < 1) {
		levelOfDetail = 1;
	}
	printf("Level of Detail: %d.\n", levelOfDetail);
	glutPostRedisplay();
}

// Standard reshape functions to change width and height as needed.  
void Scene::reshape(int w, int h) {
	width = w;
	height = h;
	glViewport( 0,0, w, h );
	glutPostRedisplay();
}


// Nothing here.  You will fill in drag for WorkingScene
void Scene::drag(int x, int y) {}

// Passive motion updates the location of the active point.  Note that we 
// normalize the given x and y values for the total width and height.  
// Note also that the height is properly inverted to take account of
// bottom to top vs top to bottom conventions.
void Scene::passiveMotion(int x, int y) {
	if (theOnlyCurve != NULL) {
		theOnlyCurve->updateActivePoint(x / (float)width, 
				(height-y) / (float)height); 
	}
	glutPostRedisplay();
}

// Nothing here.  You will fill in mouse for WorkingScene
void Scene::mouse(int button, int state, int x, int y) {}

// Since these are all static, they are set at the top of the file.
Scene::Scene() {
}

Scene::~Scene() {

}

