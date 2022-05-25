#include "DataFile.h"
#include <fstream>
using namespace std;

// function to load image from binary file and display on screen
Image LoadImageEx(Color* pixels, int width, int height)
{
	Image image = { 0 };
	unsigned char* fileData = (unsigned char*)pixels;
	unsigned char* dataPtr = fileData;
	unsigned int size = GetPixelDataSize(width, height, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
	image.data = RL_MALLOC(size);      // Allocate required memory in bytes
	if (image.data)
	{
		memcpy(image.data, dataPtr, size); // Copy required data to image
		image.width = width;
		image.height = height;
		image.mipmaps = 1;
		image.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
	}
	return image;
}

Color* GetImageData(Image image)
{
    return (Color*)image.data;
}

DataFile::DataFile()
{
	recordCount = 0;
}

DataFile::~DataFile()
{

}

DataFile::Record* DataFile::GetRecord(int index)
{
	return &record;
}

void DataFile::Load(string filename, unsigned int index)
{
	// read info from file into program
	ifstream infile(filename, ios::binary);

	infile.read((char*)&recordCount, sizeof(int));

	for (int i = 0; i <= index; i++)
	{	
		// initialise variables and fill with data from file
		int nameSize = 0;
		int ageSize = 0;
		int width = 0, height = 0, format = 0, imageSize = 0;

		infile.read((char*)&width, sizeof(int));
		infile.read((char*)&height, sizeof(int));

		imageSize = sizeof(Color) * width * height;

		infile.read((char*)&nameSize, sizeof(int));
		infile.read((char*)&ageSize, sizeof(int));

		if (index == i)
		{
			char* imgdata = new char[imageSize];
			infile.read(imgdata, imageSize);

			// Load image from binary file
			Image img = LoadImageEx((Color*)imgdata, width, height);

			// ITEM 2: Add 1 to get correct space and ignore data in array until you reach nullptr
			char* name = new char[nameSize + 1];
			name[nameSize] = '\0';

			int age = 0;

			infile.read((char*)name, nameSize);
			infile.read((char*)&age, ageSize);

			//ITEM 1: fill record details once the correct index has been found in the for loop
			record.image = img;
			record.name = (string)name;
			record.age = age;

			//delete data after use
			delete[] imgdata;
			delete[] name;
		}
		
		//ITEM 1: if the corrct index isn't found then seek by adding total bytes from the current position
		else
		{			
			infile.seekg((ageSize + nameSize + imageSize), ios::cur);
		}				
	}
	infile.close();
}