#include "main.h"
#include "lodepng.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "toojpeg.h"
using namespace std;

ofstream myNewFile("picture.jpg", std::ios_base::out | ios::binary);

int main(int argc, char** argv)
{
	if (argc == 1)
	{
		cout << "Unzulässige Syntax, bitte geben sie einen gülltigen Bildpfad an." << endl;
		return EXIT_FAILURE;
	}

	const char* myFile = "picture.png";
	ifstream stream(myFile, ios::in | ios::binary);

	unsigned char data[8];
	stream.read((char*)data, 8);

	// Check if the signature of png is given
	int png_signature[8] = { 137, 80, 78, 71, 13, 10, 26, 10 };

	for (int i = 0; i < 8; i++)
	{
		if (int(data[i]) != png_signature[i])
		{
			cout << "Die PNG-Signatur konnte nicht gefunden werden. Sicher das sie eine .png - Datei laden?" << endl;
			stream.close();
			return 0;
		}
	}

	unsigned short width;
	unsigned short height;
	vector<Pixel> png_data = loadPNGToVector(myFile, width, height);
	createJPG(png_data, width, height);

	stream.close();

	return EXIT_SUCCESS;
}

void writeByte(unsigned char byte)
{
	myNewFile << byte;
}

void createJPG(vector<Pixel> png_data, unsigned short width, unsigned short height)
{
	const auto bytesPerPixel = 3;

	// allocate memory
	auto image = new unsigned char[width * height * bytesPerPixel];

	int pixel = 0;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			// memory location of current pixel
			auto offset = (y * width + x) * bytesPerPixel;

			// Set pixels in image
			image[offset] = png_data[pixel].r;
			image[offset + 1] = png_data[pixel].g;
			image[offset + 2] = png_data[pixel].b;

			pixel++;
		}
	}

	TooJpeg::writeJpeg(writeByte, image, width, height);
	delete[] image;
}

vector<Pixel> loadPNGToVector(const char* filename, unsigned short &w, unsigned short &h) {
  std::vector<unsigned char> image; // the raw pixels

  unsigned width, height;

  // decode
  unsigned error = lodepng::decode(image, width, height, filename);

  // if there's an error, display it
  if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

  w = width;
  h = height;

  vector<Pixel> tmp;

  for (unsigned int i = 0; i < image.size(); i += 4)
  {
	  Pixel p;
	  p.r = (int)image[i];
	  p.g = (int)image[i+1];
	  p.b = (int)image[i+2];
	  p.a = (int)image[i+3];

	  // Set field color to white if alpha is 0
	  if (p.a == 0)
	  {
		  p.r = 255;
		  p.g = 255;
		  p.b = 255;
		  p.a = 255;
	  }

	  tmp.push_back({p.r, p.g, p.b, p.a});
  }

  return tmp;
}