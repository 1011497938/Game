#include "shader.h"
#include <fstream>
#include <iostream>
Shader::~Shader()
{
	glUseProgram(0);
	glDeleteProgram(proID);
}

GLuint Shader::createObject(const GLenum& shaderType, const char* strShaderFile)
{
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &strShaderFile, NULL);
	glCompileShader(shader);
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);
		const char* strShaderType = NULL;
		switch (shaderType)
		{
		case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
		case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
		case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
		}
		Error e((string() + strShaderType + strInfoLog).c_str());
		delete strInfoLog;
		throw e;
	}
	shaderList.push_back(shader);

	return shader;
}
 
GLuint Shader::createObject(const GLenum& shaderType, const string& fileName)
{
	ifstream fin;
	fin.open(fileName, ios::binary);
	if (!fin.is_open())
		throw Error();
	fin.seekg(0, ios::end);
	streamsize total_bytes = fin.tellg();
	char* strShaderFile = new char[total_bytes + 1];
	fin.seekg(0, ios::beg);
	fin.read(strShaderFile, total_bytes); 
	strShaderFile[total_bytes] = 0;
	int shader = Shader::createObject(shaderType, strShaderFile);
	fin.close();
	delete strShaderFile;
	
	return shader;
}
 
GLuint Shader::createProgram()
{
	proID = glCreateProgram();
	for (vector<GLuint>::const_iterator i = shaderList.begin(); i != shaderList.end(); i++)
		glAttachShader(proID, *i);
	glLinkProgram(proID);
	GLint status;
	glGetProgramiv(proID, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(proID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(proID, infoLogLength, NULL, strInfoLog);
		Error e(strInfoLog);
		delete strInfoLog;
		throw e;
	}
	else
	{
		for (vector<GLuint>::const_iterator i = shaderList.begin(); i != shaderList.end(); i++)
			glDeleteShader(*i);
		shaderList.erase(shaderList.begin(), shaderList.end());
	}

	return proID;
}

void Shader::useProgram() const
{
	if (proID != 0)
		glUseProgram(this->proID);
	else
		throw Error();
}

const GLuint& Shader::getID() const
{
	return proID;
}