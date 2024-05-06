#if !defined(BEZIER_H)
#define BEZIER_H

#include "Curve.h"

// The Bezier class.  It just adds the draw method to Curve.  
// This is a basic Bezier, can be implemented with deCasteljau.

class Bezier : public Curve  
{
	public:
		void draw(int levelOfDetail);
		Bezier() {};
		virtual ~Bezier() {};

};

#endif
