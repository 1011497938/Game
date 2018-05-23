#pragma once

#include <SOIL.h>

#include "resource.h"
#include "stdh.h"
using namespace std;

class Texture
{
public:
	enum TextureType { TEXTURE_SHADOW, TEXTURE_DIFFUSE, TEXTURE_SPECULAR };
	Texture() : id(0) { type = TEXTURE_DIFFUSE; }
	Texture(GLuint id) : id(id) { type = TEXTURE_SHADOW; }
	~Texture();
	void load2DTexture(const char* filepath);
	void bind(const TextureType& type) const;
	inline const GLuint& getID() const { return id; };
	inline const TextureType& getType() const { return type; }

private:
	GLuint id;
	TextureType type;
	std::string path;
};
