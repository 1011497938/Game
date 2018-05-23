#pragma once

#include "Model/model.h"
#include "Model/animModel.h"
#include "stdh.h"
#include "texture.h"
#include "shader.h"
#include "camera.h"
#include "light.h"
#include <glm\glm.hpp>
#include <map>
#include <sys/timeb.h>

const int GLFLOAT_SIZE = sizeof(GLfloat);

class Renderer
{
public:
	Renderer(const Camera& camera, const Shader& shader);
	~Renderer();

	void addObject(Object* m);
	static float getRunningTime();
	void renderDepthMap(const glm::mat4& depthVP, const Shader& depthShader, const int& flag = 1);
	void render();
	void setCenterPos(glm::vec3& c) { center = c; }
	
	vector<Object*> objects;
	static timeb start;
private:
	glm::vec3 center;
	const Camera& camera;
	const Shader& shader;
};

struct Character {
	Texture TextureID;   // ID 字形纹理
	glm::ivec2 Size;    // 字形尺寸
	glm::ivec2 Bearing; // 向上的偏移量
	GLuint Advance;     // 水平偏移量
};

/*class BaseRenderer
{
public:
	BaseRenderer(Model& m) :m(m) {}
	~BaseRenderer() {}

	virtual void renderDepthMap(const glm::mat4& depthVP, const Shader& depthShader) = 0;
	Model& m;

private:
	glm::mat4 depthBiasMVP;
};
class TextRenderer : public BaseRenderer
{
public:
	const Shader& fontShader;

	TextRenderer(const Camera& camera, const Shader& fontShader);
	~TextRenderer();
	void load(const std::string& font, const GLuint& fontSize);
	void renderText(const std::string& text, const glm::vec3& pos, const bool& onScreen = true, const glm::vec3& color = glm::vec3(1.0f), const GLfloat& scaleSize = 1.0);

private:
	GLuint vboID;
	std::map<GLchar, Character*> Characters;
};*/

