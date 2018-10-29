#include "Texture.hpp"
#include "FreeImage/FreeImage.h"
#include <iostream>

using namespace std;
using namespace cgue;

string datensatzDir = "Datensatz";

// Nur Libraries zum Laden der Texture in den Hauptspeicher verwenden,
// nicht direkt in die Grafikkarte !!! (Vorgabe von CGUE)

Texture::Texture(const std::string& modelDir, const std::string& path)
{
	string pathWithDir = datensatzDir + "/" + modelDir + "/" + path;

	// Texture im RAM
	glGenTextures(1, &handle);

	// Texture-Slot in der Grafikkarte
	glActiveTexture(GL_TEXTURE3);
	// Define that texture type of the slot to use
	glBindTexture(GL_TEXTURE_2D, handle);

	const char * filename = pathWithDir.c_str();

	cout << "FILENAME: " << filename << endl;

	//cout << "filename " << filename << endl;


	//image format
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	//pointer to the image, once loaded
	FIBITMAP *dib(0);
	//pointer to the image data
	BYTE* bits(0);
	//image width and height
	unsigned int width(0), height(0);
	//OpenGL's image ID to map to
	GLuint gl_texID;

	//check the file signature and deduce its format
	fif = FreeImage_GetFileType(filename, 0);
	//if still unknown, try to guess the file format from the file extension
	if (fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(filename);
	//if still unkown, return failure
	if (fif == FIF_UNKNOWN) {
		cout << "FIF_UNKNOWN" << endl;
		return;
	}

	//check that the plugin has reading capabilities and load the file
	if (FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, filename);
	//if the image failed to load, return failure
	if (!dib) {
		cout << "!dib" << endl;
		return;
	}

	//retrieve the image data
	bits = FreeImage_GetBits(dib);
	if (bits == NULL)
	{
		cout << "!bits" << endl;
		return;
	}
	//get the image width and height
	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);

	cout << "width: " << width << endl;
	cout << "height: " << height << endl;

	/*
	cout << "width" << width << endl;
	cout << "height" << height << endl;
	cout << "bits " << bits << endl;
	*/

	// TODO: Look up in API, Load texture binary data from program to graphics card
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, bits);

	// TODO: Look up
	glGenerateMipmap(GL_TEXTURE_2D);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Interpoliert
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	//cout << "Fertig geladen!" << endl;
}

Texture::~Texture()
{
	glDeleteTextures(1, &handle);
}

void Texture::bind(int unit)
{
	glActiveTexture(GL_TEXTURE0 + unit);


	auto min = GL_LINEAR;
	if (MIP_MAPPING_QUALITY == 0 && TEXTURE_SAMPLING_QUALITY)
	{
		min = GL_LINEAR;
	}
	else if (MIP_MAPPING_QUALITY == 0 && !TEXTURE_SAMPLING_QUALITY)
	{
		min = GL_NEAREST;
	}
	else if (MIP_MAPPING_QUALITY == 1 && TEXTURE_SAMPLING_QUALITY)
	{
		min = GL_LINEAR_MIPMAP_NEAREST;
	}
	else if (MIP_MAPPING_QUALITY == 1 && !TEXTURE_SAMPLING_QUALITY)
	{
		min = GL_NEAREST_MIPMAP_NEAREST;
	}
	else if (MIP_MAPPING_QUALITY == 2 && TEXTURE_SAMPLING_QUALITY)
	{
		min = GL_LINEAR_MIPMAP_LINEAR;
	}
	else if (MIP_MAPPING_QUALITY == 2 && !TEXTURE_SAMPLING_QUALITY)
	{
		min = GL_NEAREST_MIPMAP_LINEAR;
	}

	auto mag = GL_LINEAR;
	if (TEXTURE_SAMPLING_QUALITY)
	{
		mag = GL_LINEAR;
	}
	else if (!TEXTURE_SAMPLING_QUALITY)
	{
		mag = GL_NEAREST;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min); // Interpoliert
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);

	glBindTexture(GL_TEXTURE_2D, handle);
}