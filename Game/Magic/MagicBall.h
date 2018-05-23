#pragma once

#include "../Model/model.h"
#include "../texture.h"
#include "../renderer.h"
#include "../callback.h"
#include "particle.h"


class Magicball : public Model {
public:
	Magicball() {
		hittedObject = NULL;
	}
	bool loadModel(const std::string& filePath);
	void draw() const;
	void drawDepthMap() const;

	void setVboData(const std::vector<Vertex>& vertData, const std::vector<GLuint>& indices);
	bool loadTexture(const std::string& filePath);

	void setdirection(const glm::vec3& positon, const glm::vec2& direction, Renderer& r);
	void flash(bool runing);
	Object* hittedObject;
public:
	GLuint VAOId, EBOId, vertexVBO;
	int size;
	float xCenter, yCenter, zCenter;
	float camx, camy, camz;
	CParticle Flame;

	Texture texture;
	Object* mball;
};
