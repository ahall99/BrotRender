#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <thread>
#include <mutex>
#include <random>
#include <algorithm>
#include "Buddhabrot.h"
#include "util.h"
#include <time.h>
#include <chrono>

using namespace vec;
using namespace util;

// -2 to 0.5, -1.2 to 1.2

/*
float c2 = dot(c, c);
		// skip computation inside M1 - http://iquilezles.org/www/articles/mset_1bulb/mset1bulb.htm
		if( 256.0*c2*c2 - 96.0*c2 + 32.0*c.x - 3.0 < 0.0 ) return 0.0;
		// skip computation inside M2 - http://iquilezles.org/www/articles/mset_2bulb/mset2bulb.htm
		if( 16.0*(c2+2.0*c.x+1.0) - 1.0 < 0.0 ) return 0.0;
	*/


vec2 fragToUv(vec2 fragCoord, vec2 res)
{
	return (2.0 * fragCoord - res) / res.y;
}

vec2 uvToFrag(vec2 uv, vec2 res)
{
	//uv.y = -uv.y;
	uv = rotate(uv, -pi / 2.0);
	uv  = uv / 1.4 / 1.1;
	uv.y = uv.y - 0.7*2.0 * 103 / 1440.0;
	vec2 fragCoord = 0.5 * (uv * res.y + res);
	return vec2(floor(fragCoord.x), floor(fragCoord.y));
} 



const int defaultResX = 500;
const int defaultResY = 500;
const int numThreads = 8;
const uint64_t trajectoriesPerThread = 800000; // about 11s per 800k trajectories
const int maxIters[3] = { 150, 1200, 3500 }; // 150 1200 3500
std::mutex m;

// TODO entire operation
void writePoints(std::vector<vec2> &points, vec2 C, vec2 realMix, vec2 imaginaryMix, vec2 resolution, uint64_t* countBuffer, int channel)
{
	m.lock();
	for (vec2 point : points)
	{
		vec2 rotatedPoint = vec2(realMix.x * point.x + realMix.y * C.x, imaginaryMix.x * point.y + imaginaryMix.y * C.y);
		vec2 writeFrag = uvToFrag(rotatedPoint, resolution);
		int x = (int)writeFrag.x;
		int y = (int)writeFrag.y;

		if (x >= 0 && x < resolution.x &&
			y >= 0 && y < resolution.y)
		{
			countBuffer[channel + int(y * resolution.x + x) * 3] += 1;
		}
	}
	m.unlock();
}

void writeTrajectories(int threadIndex,
	vec2 z0,
	vec2 realMix,
	vec2 imaginaryMix,
	vec2 resolution,
	uint64_t* countBuffer,
	std::default_random_engine engine,
	std::uniform_real_distribution<double> xDistribution,
	std::uniform_real_distribution<double> yDistribution)
{
	uint64_t trajectories = 0;
	while (trajectories < trajectoriesPerThread)
	{
		//vec2 C = vec2(xDistribution(engine), yDistribution(engine)); // Mandelbrot
		/*
		double minR = pow(4.0, -1.0 / 3.0) - pow(4.0, -4.0 / 3.0);
		double r = 2.4 * (xDistribution(engine)) + minR; // TODO TENDED TOWARD CENTER
		double theta = twoPi * yDistribution(engine);
		vec2 C = r * vec2(cos(theta), sin(theta));/**/
		//vec2 z0 = vec2(xDistribution(engine), yDistribution(engine));

		// u(theta) / (k^(1/k-1)) -  u(k*theta) / (k^(k/k-1))
		const double k = 4.0; // todo this will change
		double boundTheta = twoPi * (xDistribution(engine) - 0.5);
		vec2 boundary = unit(boundTheta) / pow(k, 1.0 / (k - 1.0))
			- unit(k * boundTheta) / pow(k, k / (k - 1.0));
		double boundR = length(boundary);
		double cTheta = atan2(boundary.y, boundary.x);
		double cR = boundR + 0.1 * yDistribution(engine);
		vec2 C = cR * unit(cTheta);


		// Skip m1 and m2 - Inigo Quilez
		/*
		double c2 = dot(C, C);
		if (256.0*c2*c2 - 96.0*c2 + 32.0*C.x - 3.0 < 0.0 ||
			16.0*(c2 + 2.0*C.x + 1.0) - 1.0 < 0.0)
			continue;/**/

		std::vector<vec2> points;
		vec2 z = z0;
		int iters;
		for (iters = 0; iters < maxIters[0]; iters++)
		{
			z = complexSquare(complexSquare(z)) + C;
			//z = complexMultiply(complexMultiply(complexMultiply(z, z), z), z) + C;
			
			if (dot(z, z) >= 16.0)
				break;
			points.push_back(z);
		}
		if (iters != maxIters[0])
		{
			writePoints(points, C, realMix, imaginaryMix, resolution, countBuffer, 0);
		}

		// BLUE
		for (; iters < maxIters[1]; iters++)
		{
			z = complexSquare(complexSquare(z)) + C;
			//z = complexMultiply(complexMultiply(complexMultiply(z, z), z), z) + C;

			if (dot(z, z) >= 16.0)
				break;
			points.push_back(z);
		}
		if (iters != maxIters[1])
		{
			writePoints(points, C, realMix, imaginaryMix, resolution, countBuffer, 1);
		}

		for (; iters < maxIters[2]; iters++)
		{
			z = complexSquare(complexSquare(z)) + C;
			//z = complexMultiply(complexMultiply(complexMultiply(z, z), z), z) + C;

			if (dot(z, z) >= 16.0)
				break;
			points.push_back(z);
		}
		if (iters != maxIters[2])
		{
			writePoints(points, C, realMix, imaginaryMix, resolution, countBuffer, 2);
		}
		trajectories++;

		/*
		if (threadIndex == 0)
		{
			for (uint64_t i = 1; i < 5; i++)
			{
				if (trajectories == i * trajectoriesPerThread / 5)
					std::cout << "Thread 0 completed " << i * 20 << "% of its trajectories\n";
			}
		}/**/
	}
}

const int newtonIters[3] = { 10, 50, 1000 };
const double thresholds[3] = { 0.0000001, 0.0000001, 0.0000001 }; // newton

//z3 + 1 / 3z2
void writeNewtonTrajectories(int threadIndex,
	vec2 z0,
	vec2 realMix,
	vec2 imaginaryMix,
	vec2 resolution,
	uint64_t* countBuffer,
	std::default_random_engine engine,
	std::uniform_real_distribution<double> xDistribution,
	std::uniform_real_distribution<double> yDistribution)
{
	uint64_t trajectories = 0;
	while (trajectories < trajectoriesPerThread)
	{
		//vec2 C = vec2(xDistribution(engine), yDistribution(engine)); // Mandelbrot
		//*
		double r = 10.0 * xDistribution(engine); // TODO TENDED TOWARD CENTER
		double theta = twoPi * yDistribution(engine);
		vec2 z0 = r * vec2(cos(theta), sin(theta));

		std::vector<vec2> points;
		vec2 z = z0;
		int iters;
		for (iters = 0; iters < newtonIters[0]; iters++)
		{
			vec2 dZ = complexDivide(
				complexMultiply(complexMultiply(z, z), z) + vec2(1.0, 0.0),
				3.0 * complexMultiply(z, z));
			if (dot(dZ, dZ) <= thresholds[0])
				break;
			z = z - dZ;
			points.push_back(z);
		}
		writePoints(points, z0, realMix, imaginaryMix, resolution, countBuffer, 0);

		// BLUE
		for (; iters < newtonIters[1]; iters++)
		{
			vec2 dZ = complexDivide(
				complexMultiply(complexMultiply(z, z), z) + vec2(1.0, 0.0),
				3.0 * complexMultiply(z, z));
			if (dot(dZ, dZ) <= thresholds[1])
				break;
			z = z - dZ;
			points.push_back(z);
		}
		writePoints(points, z0, realMix, imaginaryMix, resolution, countBuffer, 1);

		for (; iters < newtonIters[2]; iters++)
		{
			vec2 dZ = complexDivide(
				complexMultiply(complexMultiply(z, z), z) + vec2(1.0, 0.0),
				3.0 * complexMultiply(z, z));
			if (dot(dZ, dZ) <= thresholds[2])
				break;
			z = z - dZ;
			points.push_back(z);
		}
		writePoints(points, z0, realMix, imaginaryMix, resolution, countBuffer, 2);

		trajectories++;

		/*
		if (threadIndex == 0)
		{
			for (uint64_t i = 1; i < 5; i++)
			{
				if (trajectories == i * trajectoriesPerThread / 5)
					std::cout << "Thread 0 completed " << i * 20 << "% of its trajectories\n";
			}
		}/**/
	}
}

void writeMapToFile(vec2 resolution, uint64_t minCounts[], uint64_t maxCounts[], uint64_t *countBuffer)
{
	int32_t resX = resolution.x;
	int32_t resY = resolution.y;
	std::ofstream output;
	std::cout << "Writing map to file\n";
	output.open("C:\\Users\\Anthony\\Desktop\\buddhabrot\\buddhabrot.bmap", std::ios::binary);
	output.write((char*)(&resX), 4);
	output.write((char*)(&resY), 4);
	output.write((char*)(minCounts), 8*3);
	output.write((char*)(maxCounts), 8*3);
	for (int y = 0; y < resY; y++)
	{
		output.write((char*)(countBuffer + resX*y*3), resX * 8 * 3);
	}
	output.close();
}

uint64_t *readMapFromFile(vec2 &resolution, uint64_t minCounts[], uint64_t maxCounts[])
{
	std::ifstream input;
	input.open("C:\\Users\\Anthony\\Desktop\\buddhabrot\\buddhabrot.bmap", std::ios::binary);
	if (!input.good())
	{
		return nullptr;
	}
	int32_t resX;
	int32_t resY;
	input.read((char*)(&resX), 4);
	input.read((char*)(&resY), 4);
	resolution = vec2(resX, resY);
	input.read((char*)(minCounts), 8*3);
	input.read((char*)(maxCounts), 8*3);

	uint64_t *countBuffer = new uint64_t[resX * resY * 3];
	for (int y = 0; y < resY; y++)
	{
		input.read((char*)(countBuffer + resX*y*3), resX * 8 * 3);
	}
	if (input.good())
	{
		std::cout << "Found and read a " << resX << " x " << resY << " map\n";
		return countBuffer;
	}
	delete[] countBuffer;
	return nullptr;
}

void writeMapToImage(vec2 resolution, uint64_t minCounts[], uint64_t maxCounts[], uint64_t *countBuffer)
{
	int32_t resX = resolution.x;
	int32_t resY = resolution.y;
	uint64_t relativeMaxes[3];

	std::cout << "Writing to image\n";
	std::ofstream output;
	output.open("C:\\Users\\Anthony\\Desktop\\buddhabrot\\buddhabrot.ppm", std::ios::binary);
	output << "P6\n";
	output << resX << ' ' << resY << '\n';
	output << "255\n";

	for (int i = 0; i < 3; i++)
		relativeMaxes[i] = maxCounts[i] - minCounts[i];

	for (int y = 0; y < resY; y++)
	{
		for (int x = 0; x < resX; x++)
		{
			float intensities[3];
			for (int i = 0; i < 3; i++)
			{
				uint64_t count = countBuffer[i + (y * resX + x) * 3];
				uint64_t relativeCount = count - minCounts[i];
				uint64_t relativeMax = relativeMaxes[i];
				double scalar = double(relativeCount + 1) / double(relativeMax + 1);
				intensities[i] = log(15.0 * scalar) / log(15.0);
				//intensities[i] = log(relativeCount + 1) / log(relativeMax + 1);
				//if (i == 0)
				//	intensities[i] = 1.45 * intensities[i] - 0.45;
				intensities[i] = std::clamp(intensities[i], 0.0f, 1.0f);

				//intensities[i] = pow(count / maxCount[i], 0.05);
			}
			//*
			vec3 color = 255.0 * vec3(
				powf(intensities[0], 1.0),  // b //    1, 1.4, 1.8!!! for purple 1, 1, 1.1
				powf(intensities[1], 1.45),  // r
				powf(intensities[2], 1.8)); // g /**/
			//vec3 color = 255.0 * vec3(intensities[0], intensities[1], intensities[2]);
			unsigned char r = (unsigned char)(round(color.x));
			unsigned char g = (unsigned char)(round(color.y));
			unsigned char b = (unsigned char)(round(color.z));

			output << g << b << r;
		}
	}
}



void Buddhabrot::renderBuddhabrot()
{
	/*vec3 q(0, 1, 2);
	std::cout << q[0] << '\n';
	std::cout << q.x << '\n';
	q[0] = 5;
	std::cout << q[0] << '\n';
	std::cout << q.x << '\n';
	std::cout << q.y << '\n';
	std::cout << q.z << '\n';*/
	
	vec2 resolution;
	uint64_t minCounts[3] = { 0xFFFFFFFFFFFFFFFF };
	uint64_t maxCounts[3] = { 0 };
	uint64_t *countBuffer = readMapFromFile(resolution, minCounts, maxCounts);
	if (countBuffer == nullptr)
	{
		//return;
		std::cout << "Map not found or could not be read, generating new map\n";
		resolution = vec2(defaultResX, defaultResY);
		countBuffer = new uint64_t[defaultResX * defaultResY * 3];
	}

	// Don't uncomment this yet!!!
	//*
	vec2 z0 = vec2(0.0); // 0.6 one axis, 0.4 both axis look cool
	double thetaReal = 0.;// 25 * pi;
	double thetaImaginary = 0.;// 25 * pi;
	vec2 realMix = vec2(cos(thetaReal), sin(thetaReal));
	vec2 imaginaryMix = vec2(cos(thetaImaginary), sin(thetaImaginary));

	std::thread threads[numThreads];

	std::default_random_engine engines[numThreads];
	std::uniform_real_distribution<double> xDistributions[numThreads];
	std::uniform_real_distribution<double> yDistributions[numThreads];

	std::cout << numThreads << " threads writing " << trajectoriesPerThread << " trajectories each\n";
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	for (int i = 0; i < numThreads; i++)
	{
		engines[i] = std::default_random_engine(time(NULL) + i);
		xDistributions[i] = std::uniform_real_distribution<double>(0.0, 1.0);
		yDistributions[i] = std::uniform_real_distribution<double>(0.0, 1.0);
		threads[i] = std::thread(writeTrajectories,
			i, z0, realMix, imaginaryMix, resolution, countBuffer, engines[i], xDistributions[i], yDistributions[i]);
	}
	for (int i = 0; i < numThreads; i++)
	{
		threads[i].join();
	}
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Finished writing trajectories after " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() / 1000.0 << " s\n";


	int resX = resolution.x;
	int resY = resolution.y;
	
	for (int i = 0; i < 3; i++)
	{
		minCounts[i] = 0xFFFFFFFFFFFFFFFF;
		maxCounts[i] = 0;
	}
	for (int y = 0; y < resY; y++)
	{
		for (int x = 0; x < resX; x++)
		{
			for (int i = 0; i < 3; i++)
			{
				uint64_t count = countBuffer[i + (y * resX + x) * 3];
				minCounts[i] = std::min(count, minCounts[i]);
				maxCounts[i] = std::max(count, maxCounts[i]);
			}
		}
	}

	writeMapToFile(resolution, minCounts, maxCounts, countBuffer);/**/

	writeMapToImage(resolution, minCounts, maxCounts, countBuffer);

	delete[] countBuffer;
}