#include "util.h"
#include <cmath>

using namespace vec;

double util::mod(double a, double b)
{
	return a - b * floor(a / b);
}

double util::fract(double x)
{
	return x - floor(x);
}

vec2 util::complexMultiply(vec2 c1, vec2 c2)
{
	return vec2(
		c1.x*c2.x - c1.y*c2.y,
		c1.x*c2.y + c1.y*c2.x);
}

vec2 util::complexSquare(vec2 c)
{
	return vec2(
		c.x*c.x - c.y*c.y,
		2.0 * c.x*c.y);
}

vec2 util::complexDivide(vec2 c1, vec2 c2)
{
	return complexMultiply(c1, vec2(c2.x, -c2.y)) / dot(c2, c2);
}