#pragma once

#include "stdh.h"
#include "glm/glm.hpp"
#include <assimp/Importer.hpp>      
#include <assimp/scene.h>           
#include <assimp/postprocess.h>
#include <vector>
#include <map>

#include <atlimage.h>

using namespace std;
class Texture;

class SubObj {
public:
	SubObj() {
		xmin = zmin = ymin = 99999;
		xmax = ymax = zmax = -99999;
	}
	void draw() const;
	void setData(const std::vector<Vertex>& vertData, const std::vector<GLuint>& indices, Texture* tex);
	~SubObj() {
		delete tex;
	}

	float xmin, xmax, ymin, ymax, zmin, zmax;
private:
	GLuint VAOId, EBOId, vertexVBO;
	int size;
	Texture *tex;
};


struct Image
{
	GLuint height, width;
	GLfloat size;
	GLubyte* data = NULL;
	~Image() { if (data) delete data; }
};

class ResourceManager
{
public:
	static int BMP_Header_Length;
	static void loadOBJ(const string& fileName, std::vector<SubObj*>& objs);
	static void writeOBJ(const string& fileName, const std::vector<Vertex>& outData, const std::vector<GLuint>& outIndices);
	static GLubyte* loadBMP(const string& fileName, GLuint& width, GLuint& height);
	static Image* loadImage(const string& fileName);
};

