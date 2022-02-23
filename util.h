// Math utility functions

#pragma once
#include "vec.h"

const double pi = 3.14159265358979323846;
const double twoPi = 2.0 * pi;

namespace util
{
	// Domain repetition
	double mod(double a, double b);

	// Domain repetition with period 1
	double fract(double x);

	vec::vec2 complexMultiply(vec::vec2 c1, vec::vec2 c2);
	vec::vec2 complexSquare(vec::vec2 c);
	vec::vec2 complexDivide(vec::vec2 c1, vec::vec2 c2);
	vec::vec2 complexPower(vec::vec2 c, double exponent);
}