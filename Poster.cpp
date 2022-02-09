#include "Poster.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <thread>
#include <cmath>
#include <algorithm>
#include "Shader.h"
#include "Buddhabrot.h"

using namespace Shader;

//*
using namespace vec;

const int defaultResX = 1800;
const int defaultResY = 1200;
//const vec2 resolution = vec2(resX, resY);

const int numThreads = 8;
//const int rowsPerBlock = 50;

/*
void renderRows(int startRow, unsigned char* writeBuffer)
{
	int y = startRow;
	for (int i = 0; y < resY && i < rowsPerThread; i++)
	{
		for (int x = 0; x < resX; x++)
		{
			vec2 fragCoord = vec2(x, resY - (y + 1)) + 0.5;

			vec3 color = Shader::mainImage(fragCoord, resolution);
			color = color * 255.0;
			color.x = round(color.x);
			color.y = round(color.y);
			color.z = round(color.z);
			int r = color.x;
			int g = color.y;
			int b = color.z;
			int pixelIndex = 3 * (i*resX + x);
			writeBuffer[pixelIndex] = (unsigned char)r;
			writeBuffer[pixelIndex + 1] = (unsigned char)g;
			writeBuffer[pixelIndex + 2] = (unsigned char)b;
		}
		y++;
	}
}*/

void shaderWorker(int threadIndex, vec2 resolution, unsigned char* image)
{
	int resX = resolution.x;
	int resY = resolution.y;
	for (int pixelIndex = threadIndex; pixelIndex < resX * resY; pixelIndex += numThreads)
	{
		int x = pixelIndex % resX;
		int y = pixelIndex / resX;
		vec2 fragCoord = vec2(x, resY - (y + 1)) + 0.5;
		vec3 color = Shader::mainImage(fragCoord, resolution);
		color = color * 255.0;
		color.x = round(color.x);
		color.y = round(color.y);
		color.z = round(color.z);
		int r = color.x;
		int g = color.y;
		int b = color.z;
		image[3 * pixelIndex] = (unsigned char)r;
		image[3 * pixelIndex + 1] = (unsigned char)g;
		image[3 * pixelIndex + 2] = (unsigned char)b;
		if (threadIndex == 0)
		{
			for (int i = 1; i < 10; i++)
			{
				if (pixelIndex == int(i * resX * resY / 10))
					std::cout << "Thread 0 at " << i * 10 << "%\n";
			}
		}
	}
}

void renderMapWorker(int threadIndex, vec2 resolution, double* map)
{
	int resX = resolution.x;
	int resY = resolution.y;
	for (int pixelIndex = threadIndex; pixelIndex < resX * resY; pixelIndex += numThreads)
	{
		int x = pixelIndex % resX;
		int y = pixelIndex / resX;
		vec2 fragCoord = vec2(x, resY - (y + 1)) + 0.5;
		vec2 fragMap = Shader::mandelbrotMap(fragCoord, resolution);
		map[2 * (x + y * resX)] = fragMap.x;
		map[1 + 2 * (x + y * resX)] = fragMap.y;
		/*
		if (threadIndex == 0)
		{
			for (int i = 1; i < 5; i++)
			{
				if (pixelIndex == int(i * resX * resY / 5))
					std::cout << "Thread 0 at " << i * 20 << "%\n";
			}
		}/**/
	}
}

void colorMapWorker(int threadIndex, vec2 resolution, double* map, unsigned char* image)
{
	int resX = resolution.x;
	int resY = resolution.y;
	for (int pixelIndex = threadIndex; pixelIndex < resX * resY; pixelIndex += numThreads)
	{
		int x = pixelIndex % resX;
		int y = pixelIndex / resX;
		double iters = map[2 * (x + y * resX)];
		double orbitDist = map[1 + 2 * (x + y * resX)];
		vec3 color  = Shader::colorMandelbrotMap(vec2(iters, orbitDist));
		color = color * 255.0;
		color.x = round(color.x);
		color.y = round(color.y);
		color.z = round(color.z);
		int r = color.x;
		int g = color.y;
		int b = color.z;
		image[3*pixelIndex] = (unsigned char)r;
		image[3*pixelIndex + 1] = (unsigned char)g;
		image[3*pixelIndex + 2] = (unsigned char)b;
	}
}

double* renderMap(vec2 resolution)
{
	int resX = resolution.x;
	int resY = resolution.y;
	double* map = new double[2 * resX * resY];
	std::thread threads[numThreads];

	std::cout << "Generating a " << resX << " x " << resY << " map\n";

	for (int i = 0; i < numThreads; i++)
	{
		threads[i] = std::thread(renderMapWorker, i, resolution, map);
	}
	for (int i = 0; i < numThreads; i++)
	{
		threads[i].join();
	}
	return map;
}

unsigned char* colorMap(vec2 resolution, double* map)
{
	int resX = resolution.x;
	int resY = resolution.y;
	unsigned char* writeBuffer = new unsigned char[3 * resX * resY];
	std::thread threads[numThreads];

	for (int i = 0; i < numThreads; i++)
	{
		threads[i] = std::thread(colorMapWorker, i, resolution, map, writeBuffer);
	}
	for (int i = 0; i < numThreads; i++)
	{
		threads[i].join();
	}
	return writeBuffer;
}

void writeMapToFile(vec2 resolution, double *map)
{
	int32_t resX = resolution.x;
	int32_t resY = resolution.y;
	std::ofstream output;
	output.open("C:\\Users\\Anthony\\Desktop\\mandelbrot\\mandelbrot.mmap", std::ios::binary);
	output.write((char*)(&resX), 4);
	output.write((char*)(&resY), 4);
	output.write((char*)(map), resX*resY * 8 * 2);
	output.close();
}

double *readMapFromFile(vec2 &resolution)
{
	std::ifstream input;
	input.open("C:\\Users\\Anthony\\Desktop\\mandelbrot\\mandelbrot.mmap", std::ios::binary);
	if (!input.good())
	{
		return nullptr;
	}
	int32_t resX;
	int32_t resY;
	input.read((char*)(&resX), 4);
	input.read((char*)(&resY), 4);
	resolution = vec2(resX, resY);

	std::cout << " Found " << resX << " x " << resY << " map\n";

	double *map = new double[resX * resY * 2];
	input.read((char*)(map), resX*resY * 8 * 2);
	if (input.good())
	{
		return map;
		input.close();
	}
	delete[] map;
	return nullptr;
}

void writeImageToFile(vec2 resolution, unsigned char *image)
{
	int resX = resolution.x;
	int resY = resolution.y;
	std::cout << "Writing to image\n";
	std::ofstream output;
	output.open("C:\\Users\\Anthony\\Desktop\\mandelbrot\\mandelbrot.ppm", std::ios::binary);
	output << "P6\n";
	output << resX << ' ' << resY << '\n';
	output << "255\n";

	output.write((char*)(image), resX * resY * 3);
	output.close();
	std::cout << "Closed image output stream\n";
}



void Mandelbrot::renderFromMap()
{
	vec2 resolution;
	double *map = readMapFromFile(resolution);
	if (map == nullptr)
	{
		int resX = defaultResX;
		int resY = defaultResY;
		resolution = vec2(resX, resY);
		map = renderMap(resolution);
		writeMapToFile(resolution, map);
	}

	unsigned char* ppm = colorMap(resolution, map);
	writeImageToFile(resolution, ppm);

	delete[] map;
	delete[] ppm;
}

void Mandelbrot::renderFromScratch()
{
	int resX = defaultResX;
	int resY = defaultResY;
	vec2 resolution = vec2(resX, resY);
	unsigned char* image = new unsigned char[3 * resX * resY];
	std::thread threads[numThreads];

	for (int i = 0; i < numThreads; i++)
	{
		threads[i] = std::thread(shaderWorker, i, resolution, image);
	}
	for (int i = 0; i < numThreads; i++)
	{
		threads[i].join();
	}
	
	writeImageToFile(resolution, image);
	delete[] image;
}