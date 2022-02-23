#include <iostream>
#include <iomanip>
#include <fstream>
#include <thread>
#include <cmath>
#include "Shader.h"
#include "Buddhabrot.h"
#include "Poster.h"

using namespace vec;
//using namespace Buddhabrot;
//using namespace Mandelbrot;

int main()
{
	/*
	double L = -0.154391744896;
	double R = -0.154382108132;
	double T = -1.030832658521;
	double B = -1.030825912786;
	double scaleInit = 0.1*0.5*abs(T - B);
	vec2 centerInit = 0.5 * vec2(L + R, T + B);

	centerInit = vec2(
	-0.15438694562691582246,
	-1.0308295015170199083);
	scaleInit = 0.1*3.0e-7;

	vec2 center = centerInit + 2.0*scaleInit * (vec2(4.5, -19 / 2.0) + vec2(0, -13.0 / 2.0)) / 600;

	std::cout << std::setprecision(20) << center.x << std::endl << center.y << std::endl << scaleInit << std::endl;/**/


	
	//Mandelbrot::renderFromMap();
	//Mandelbrot::renderFromScratch();


	todoNamespace::renderBuddhabrot();


	return 0;
}

