#pragma once

#include "model.h"
#include <sstream>

class BaseModel : public Model
{
public:
	bool loadModel(const std::string& modelType);
	void setTexture(Texture* tex) { this->tex = tex; }
	void draw() const;
	void drawDepthMap() const;

private:
	void setVboData(const std::vector<Vertex>& vertData, const std::vector<GLuint>& indices);
	bool loadCube(stringstream& s);
	GLuint VAOId, EBOId, vertexVBO;
	int size;
	Texture *tex;
};

