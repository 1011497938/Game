#pragma once

#include "../Model/model.h"
#include "../texture.h"
#include "../renderer.h"



class SkyboxFace :public Model {

};

class SkyboxTop :public Model
{
public:
	bool loadModel(const std::string& filePath);
	void draw() const;
	void drawDepthMap() const;

	void setVboData(const std::vector<Vertex>& vertData, const std::vector<GLuint>& indices);
	bool loadTexture(const std::string& filePath);

private:
	GLuint VAOId, EBOId, vertexVBO;
	int size;
	Texture texture;
};

class SkyboxFront :public Model
{
public:
	bool loadModel(const std::string& filePath);
	void draw() const;
	void drawDepthMap() const;

	void setVboData(const std::vector<Vertex>& vertData, const std::vector<GLuint>& indices);
	bool loadTexture(const std::string& filePath);

private:
	GLuint VAOId, EBOId, vertexVBO;
	int size;
	Texture texture;
};

class SkyboxBack :public Model
{
public:
	bool loadModel(const std::string& filePath);
	void draw() const;
	void drawDepthMap() const;

	void setVboData(const std::vector<Vertex>& vertData, const std::vector<GLuint>& indices);
	bool loadTexture(const std::string& filePath);

private:
	GLuint VAOId, EBOId, vertexVBO;
	int size;
	Texture texture;
};

class SkyboxLeft :public Model
{
public:
	bool loadModel(const std::string& filePath);
	void draw() const;
	void drawDepthMap() const;

	void setVboData(const std::vector<Vertex>& vertData, const std::vector<GLuint>& indices);
	bool loadTexture(const std::string& filePath);

private:
	GLuint VAOId, EBOId, vertexVBO;
	int size;
	Texture texture;
};

class SkyboxRight :public Model
{
public:
	bool loadModel(const std::string& filePath);
	void draw() const;
	void drawDepthMap() const;

	void setVboData(const std::vector<Vertex>& vertData, const std::vector<GLuint>& indices);
	bool loadTexture(const std::string& filePath);

private:
	GLuint VAOId, EBOId, vertexVBO;
	int size;
	Texture texture;
};

class Skybox {
public:
	Skybox(Renderer& r) {
		SkyboxTop *s1 = new SkyboxTop();
		SkyboxFront *s2 = new SkyboxFront();
		SkyboxBack *s3 = new SkyboxBack();
		SkyboxLeft *s4 = new SkyboxLeft();
		SkyboxRight *s5 = new SkyboxRight();
		s1->loadModel("");	s1->loadTexture("Resource\\sky\\top.jpg");
		s2->loadModel("");	s2->loadTexture("Resource\\sky\\back.jpg");
		s3->loadModel("");	s3->loadTexture("Resource\\sky\\front.jpg");
		s4->loadModel("");	s4->loadTexture("Resource\\sky\\left.jpg");
		s5->loadModel("");	s5->loadTexture("Resource\\sky\\right.jpg");
		facesObj.resize(5);
		facesObj[0] = new Object(s1);
		facesObj[1] = new Object(s2);
		facesObj[2] = new Object(s3);
		facesObj[3] = new Object(s4);
		facesObj[4] = new Object(s5);
		for (Object* o : facesObj) {
			o->setModelMat(glm::vec3(0.0f, 0.0f, 100.0f));
			o->setMaterial(glm::vec3(1.0f), glm::vec3(1.0f), 50);
			r.addObject(o);
		}
	}
private:
	vector<Object*> facesObj;
};
