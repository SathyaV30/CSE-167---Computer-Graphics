#if !defined(POINT_H)
#define POINT_H

// The basic Point Class.  
// ACTIVE_RADIUS is just how big we draw the red circle around active points 
//    which is set to 0.05 in Point.cpp
// x and y are simply spatial locations 
// active says if the point is currently active.
// draw draws the point. 
// There's also a constructor that instantiates it.  

class Point  
{
	public:
		static const float ACTIVE_RADIUS;

		float x;
		float y;
		bool active;

		// Should be GLuint, but there's no difference. This avoids dealing with the chaos of file inclusions.
		unsigned int colorPos;

		void draw();
		Point(float xpos, float ypos);
		virtual ~Point();
};

#endif

