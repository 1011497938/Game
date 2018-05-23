#pragma once

#include <string>
#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <GL\glut.h>
#include "glm\glm.hpp"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

class Error {
	std::string err;
public:
	Error(const char* e = "Error") :err(e) {}
	const std::string& getInfo() { return err; }
};
