#include "Shader.h"
#include "util.h"
#include <cmath>
#include <algorithm>

using namespace vec;
using namespace util;
using namespace std;

const int SS = 3;
const float bound = 128.0;

// lightning ~100k?
/*
const double L = -0.109331824;
const double R = -0.109327731;
const double T = -0.894943939;
const double B = -0.894942664;
const double SCALE = 0.5*abs(T - B);
const vec2 CENTER = 0.5 * vec2(L + R, T + B);
const int maxIters = 50000;/**/

// EMILY UWUWUWUWUW (zoom out for nebula without minibrot)
/*
const vec2 CENTER = vec2(
	-0.15438694517691581298,
	-1.0308295031170198186);
const double SCALE = 2.65e-7;
const int maxIters = 20000; // 2k for decent image, 200k in final
/**/

// nebula 3k? i forgot tbh
/*
const double L = -0.154391744896;
const double R = -0.154382108132;
const double T = -1.030832658521;
const double B = -1.030825912786;
const double SCALE = 0.1*0.5*abs(T - B);
const vec2 CENTER = 0.5 * vec2(L + R, T + B);
const int maxIters = 3000;/**/

// shell >> 10k
/*
const double L = -0.154386954937;
const double R = -0.154386954881;
const double T = -1.030829519994;
const double B = -1.030829519954;
const double SCALE = 0.5*abs(T - B);
const vec2 CENTER = 0.5 * vec2(L + R, T + B);
const int maxIters = 15000;/**/

// genesis ~8k
/*
const double L = -0.154387058563;
const double R = -0.154386829567;
const double T = -1.030829579955;
const double B = -1.030829419659;
const double SCALE = 0.1*0.5*abs(T - B);
const vec2 CENTER = 0.5 * vec2(L + R, T + B);
const int maxIters = 8000;/**/

// Julia (-0.22, 0.78)
/*
const double L = 0.072655716477;
const double R = 0.183168113515;
const double T = -0.699998971105;
const double B = -0.622640293179;
const double SCALE = 0.5*abs(T - B);
const vec2 CENTER = 0.5 * vec2(L + R, T + B);
const int maxIters = 2000;
const vec2 juliaC = vec2(-0.22, 0.78)/**/

// Hyperspace 40k
/*

const double L = 0.250080340145;
const double R = 0.250080766459;
const double T = 0.000001079726;
const double B = 0.000001239446;
const double SCALE = 0.5*abs(T - B);
const vec2 CENTER = 0.5 * vec2(L + R, T + B);
const int maxIters = 40000;/**/

// Hyperspace Julia
//*
const double SCALE = 0.93;
const vec2 CENTER = vec2(0.0);
const int maxIters = 200000;
const vec2 juliaC = vec2(0.25008055, 0.00000115);/**/

// trifecta 1 ~10k
/*
const double L = -0.209031343135;
const double R = -0.209031330643;
const double T = -0.827560630227;
const double B = -0.827560621482;
const double SCALE = 0.5*abs(T - B);
const vec2 CENTER = 0.5 * vec2(L + R, T + B);
const int maxIters = 20000;/**/

// Claws
/*
const double L = -0.209169953011;
const double R = -0.208841081619;
const double T = -0.827683381493;
const double B = -0.827453171518;
const double SCALE = 0.5*abs(T - B);
const vec2 CENTER = 0.5 * vec2(L + R, T + B);
const int maxIters = 3000;/**/

// Spirals
/*
const double L = -1.249756055283;
const double R = -1.249599691981;
const float T = -0.027542959371;
const float B = -0.027596631182;
const double SCALE = 0.1*0.5*abs(T - B);
const vec2 CENTER = 0.5 * vec2(L + R, T + B);
const int maxIters = 15000;/**/

// SUPER spiral
/*
const double L = -1.35049104;
const double R = -1.349583361;
const double T = -0.04291665;
const double B = -0.04263379;
const double SCALE = 0.1*0.5*abs(T - B);
const vec2 CENTER = 0.5 * vec2(L + R, T + B);
const int maxIters = 10000;/**/

// Seahorse
/*
const double L = -0.78688089;
const double R = -0.78030069;
const double T = -0.13878563;
const double B = -0.13652697;
const double SCALE = 0.1*0.5*abs(T - B);
const vec2 CENTER = 0.5 * vec2(L + R, T + B);
const int maxIters = 2000;/**/

// shadertoy <3
/*
const double L = -1.5786356;
const double R = -1.5720554;
const double T = 0.0011;
const double B = -0.0011;
const int maxIters = 3000;/**/

//const double SCALE = 0.1*0.5*abs(T - B);
//const vec2 CENTER = 0.5 * vec2(L + R, T + B);

//const double SCALE = 0.25*0.00131604;
//const vec2 CENTER = vec2(-1.575705, 0.0);//vec2(-0.5, 0);








double triangleOrbit(vec2 point)
{
	//return dot(point, point);
	double r = length(point);

	double theta = atan2(point.y, -point.x);
	theta = mod(theta + pi / 3.0, twoPi / 3.0) - pi / 3.0;

	return r * cos(theta);
}

double starOrbit(vec2 point)
{
	double theta = atan2(point.y, point.x);
	theta = mod(theta + pi / 5.0, twoPi / 5.0) - pi / 5.0;

	double r = length(point);

	point = r * vec2(cos(theta), sin(theta));

	point.y = abs(point.y);
	point = rotate(point, -twoPi / 5.0);


	return point.x;
}

// 
double leafOrbit(vec2 point)
{
	//const float offset = 2.0; //  -1.65 -0.45 2.0
	double offset = 2.0;
	//const float offset = 2.0;
	point.y = abs(point.y);
	point.y += offset;

	double leafDist = length(point) - offset;
	return leafDist;
}

vec3 colorBand(double t, vec3 exponents)
{
	double phase = t * twoPi;
	double intensity = 0.5 - 0.5 * cos(phase);
	return vec3(
		pow(intensity, exponents.x),
		pow(intensity, exponents.y),
		pow(intensity, exponents.z));
}

float mandelbrotAlpha(vec2 C)
{
	vec2 z = C;
	int iters;
	for (iters = 0; iters < maxIters; iters++)
	{
		z = complexMultiply(z, z) + C;
		if (dot(z, z) > bound * bound)
			break;
	}
	return float(iters == maxIters);
}


// Returns iters, orbit dist
vec2 mandelbrot(vec2 C)
{
	vec2 z = vec2(C.y, C.x); C = juliaC;
	double outerDist = 1e5; // Orbit trap outside the set
	double innerDist = 1e5; // Orbit trap inside the set
	int iters;
	double zz; // dot(z, z)


	// The magnitude at which we consider a point divergent. Higher values are better
	// for the smooth iteration count method (so Z^2 becomes much more significant than C)
	// at the cost of a few more iterations.
	//const float bound = 128.0; // TODO RAISE

	// This offset corrects the optimized version of the smooth iteration formula
	// so it is equivalent to the non-optimized version
	const float smoothOffset = -log2(log2(bound)) - 2.0;

	for (iters = 1; iters < maxIters; iters++)
	{
		z = complexMultiply(z, z) + C;
		zz = dot(z, z);
		if (zz > bound * bound)
			break;

		/*
		const double combScale = 1.0;
		double outer = triangleOrbit(z);
		//double logComb = abs(mod(2.0 * combScale * log2(outer)/log2(bound), 2.0) - 1.0); // tri
		double logComb = 0.5 + 0.5 *(cos(twoPi * combScale*log2(outer) / log2(bound))); // wave
		outerDist = min(outerDist, logComb);/**/

		//innerDist = min(innerDist, zz);
	}
	if (iters == maxIters) // In set
	{
		return vec2(iters, innerDist);
	}
	else // Not in set
	{
		// Smooth iteration count method by Inigo Quilez
		// https://iquilezles.org/www/articles/mset_smooth/mset_smooth.htm

		double smoothIters = iters - (log2(log2(zz)) + smoothOffset);
		return vec2(smoothIters, outerDist);
	}
}

vec3 Shader::colorMandelbrotMap(vec2 map)
{
	double iters = map.x;
	double orbitDist = map.y;
	vec3 color = vec3(0.0);

	if (iters == maxIters) // In set
	{
		//double t = 0.5*log2(orbitDist);
		//color = colorBand(t, 2.0 * vec3(3.0, 10.0, 0.75));
		//color = colorBand(t, 2.0 * vec3(10.0, 2.5, 0.75));
	}
	else // Not in set
	{
		//double t = log2(iters) + 2.0*log2(orbitDist + 0.002) + 0.9; // STAR
		//double t = log2(iters + 1.0) + log2(orbitDist + 0.05) + 0.3; // THIS IS IT CHIEF
		//double t = 0.25 * log2(iters) + 0.7;
		double t = 0.04 * iters - 0.1;
		color = colorBand(t, vec3(10.0, 2.5, 0.75)); // ATMOSPHERE BLUE
		//color = colorBand(t, vec3(1.0, 7.5, 15.0)); // EMILY RED 6
	}
	color.x = clamp(color.x, 0.0, 1.0);
	color.y = clamp(color.y, 0.0, 1.0);
	color.z = clamp(color.z, 0.0, 1.0);
	return color;
}

vec2 Shader::mandelbrotMap(vec2 fragCoord, vec2 resolution)
{
	vec2 uv = (2.0 * fragCoord - resolution) / resolution.y;
	vec2 C = CENTER + SCALE * uv;
	return mandelbrot(C);
}


vec3 Shader::mainImage(vec2 fragCoord, vec2 resolution)
{
	vec3 color = vec3(0.0);

	for (int y = 0; y < SS; y++)
	{
		for (int x = 0; x < SS; x++)
		{
			const float increment = 1.0 / SS;
			const float offset = 0.5 * increment - 0.5;
			vec2 subFragCoord = fragCoord + offset + increment * vec2(x, y);

			color = color + colorMandelbrotMap(mandelbrotMap(subFragCoord, resolution));
			//color = color + vec3(mandelbrotAlpha(CENTER + SCALE * (2.0 * subFragCoord - resolution) / resolution.y));
		}
	}
	color = color / (SS * SS);
	
	return color;
}