#pragma once

#include "stdh.h" 
#include <vector>
using namespace std;

class Shader
{
private:
	GLuint proID;
	vector<GLuint> shaderList;
public:
	Shader() :proID(0) {}
	~Shader();
	GLuint createObject(const GLenum& shaderType, const char* strShaderFile);
	GLuint createObject(const GLenum& shaderType, const string& fileName);
	GLuint createProgram();
	void useProgram() const;
	const GLuint& getID() const;
};
