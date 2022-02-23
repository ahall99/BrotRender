#pragma once

#include "vec.h"

namespace todoNamespace {
	void renderBuddhabrot();
}

class FractalHistogram {
public:
	virtual void writeTrajectories() = 0;
protected:
	const static int threadCount = 8;
};

class Buddhabrot : public FractalHistogram {

};

class NewtonHistogram : public FractalHistogram {

};