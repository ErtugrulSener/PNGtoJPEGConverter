#pragma once

#include <vector>

using namespace std;

struct Pixel
{
	int r;
	int g;
	int b;
	int a;
};

vector<Pixel> loadPNGToVector(const char* filename, unsigned short&width, unsigned short &height);
void createJPG(vector<Pixel> png_data, unsigned short width, unsigned short height);