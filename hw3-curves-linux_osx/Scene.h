#if !defined(CLASS_SCENE)
#define CLASS_SCENE

#include "Curve.h"
// OSX systems require its own headers
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif
#include <stdlib.h>

// Scene is the basic class that handles the UI.  
// height, width, are the height and width of the window 
// oldx and oldy are the old values one clicked on for ease of use.  
// The Scene also has associated with it theOnlyCurve, which is the curve 
// The remaining functions are all for user interaction 
// Since we'll have only one scene/working scene/curve, static fns are used.

class Scene {
	public:
		static GLuint colorPos; // Needed by the curve and points so they can set the proper color when being drawn
		static int height;
		static int width;
		static int oldx;
		static int oldy;

		static int levelOfDetail;

		static Curve * theOnlyCurve;
		Scene();
		virtual ~Scene();
		static void keyboard(unsigned char key, int x, int y);
		static void reshape(int w, int h);
		static void passiveMotion(int x, int y);
		static void drag(int x, int y);
		static void mouse(int button, int state, int x, int y);


};

#endif // !defined(CLASS_SCENE)

