#pragma once

#include "stdh.h"
#include "shader.h"
#include "camera.h"
#include "light.h"

class Shadow
{
public:
	Shadow(GLFWwindow* window, const Shader& shader, const int& width, const int& height);
	~Shadow();
	void getDepthMap();
	void getDepthMapEND();
	void bind();
	glm::mat4 getDepthVP(const Lights& lights, const glm::vec3& npos = glm::vec3(0.0f), bool flag = false);
	inline const void setTarget(glm::vec3& target) { this->target = target; }
	inline const Shader& getShader() { return shader; }
	inline const GLuint& getShadowID() { return shadowID; }
private:
	GLuint fboID, shadowID;
	int width, height;
	glm::vec3 target;
	GLFWwindow* window;
	const Shader& shader;
};