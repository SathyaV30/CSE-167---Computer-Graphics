#if !defined(BEZIER2_H)
#define BEZIER2_H

#include "Curve.h"

// The basic Bezier2 class.  Note that levelofdetail means differently here. 
// In this case, you will draw the curve by repeated subdivision.

class Bezier2 : public Curve  
{
	public:
		void draw(int levelOfDetail);
		Bezier2() {};
		virtual ~Bezier2() {};

};

#endif
