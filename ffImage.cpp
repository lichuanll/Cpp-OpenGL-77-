#include "ffImage.h"
#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"
ffImage* ffImage::readFromFile(const char* _filename)
{
	int picType = 0;
	int width = 0;
	int height = 0;

	//stbi_set_flip_vertically_on_load(true);


	unsigned char* bits = stbi_load(_filename, &width, &height, &picType, STBI_rgb_alpha);
	/*for (int i = 0; i < width * height * 4; i += 4)
	{
		unsigned char temp = bits[i];
		bits[i] = bits[i + 2];
		bits[i + 2] = temp;
	}*/

	ffImage* _image = new ffImage(width, height, picType, (ffRGBA*)bits);

	stbi_image_free(bits);
	return _image;

}