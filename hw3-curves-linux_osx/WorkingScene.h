#if !defined(WORKINGSCENE_H)
#define WORKINGSCENE_H

#include "Scene.h"

// Working Scene refers to the Scene object.  In particular, 
// it adds functions for drag and mouse, that are important to draw the points
// You will need to fill in these functions.

class WorkingScene : public Scene  
{
	public:
		static void drag(int x, int y);
		static void mouse(int button, int state, int x, int y);

		WorkingScene() {};
		virtual ~WorkingScene() {};

};

#endif


