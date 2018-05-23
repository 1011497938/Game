#include "texture.h"
#include <fstream>

#pragma comment(lib, "SOIL.lib")

Texture::~Texture()
{
	glDeleteTextures(1, &id);
}

void Texture::load2DTexture(const char* filepath)
{
	//GLuint w, h;
	//GLubyte* p;
	this->id = SOIL_load_OGL_texture
	(
		filepath,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_TEXTURE_REPEATS//SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	if (this->id == 0)
	{
		printf("SOIL loading error: '%s'\n", SOIL_last_result());
	}

	//p = ResourceManager::loadBMP(filepath, w, h);
	//generate(w, h, p);
	//delete p;
	this->path = filepath;
}

void Texture::bind(const TextureType& type) const
{
	switch (type) {
	case TEXTURE_SHADOW:
		glActiveTexture(GL_TEXTURE0);
		break;
	case TEXTURE_DIFFUSE:
		glActiveTexture(GL_TEXTURE1);
		break;
	case TEXTURE_SPECULAR:
		glActiveTexture(GL_TEXTURE2);
		break;
	}
	glBindTexture(GL_TEXTURE_2D, this->id);
}