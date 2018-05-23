#pragma once

#include "shadow.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>

Shadow::Shadow(GLFWwindow* window, const Shader& shader, const int& width, const int& height) :window(window), shader(shader), width(width), height(height)
{
	glGenTextures(1, &shadowID);
	glBindTexture(GL_TEXTURE_2D, shadowID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);// GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//GL_NEAREST 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//GL_NEAREST 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffersEXT(1, &fboID);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboID);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, shadowID, 0);
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if (status != GL_FRAMEBUFFER_COMPLETE_EXT)
		throw Error();
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	target = glm::vec3(40.0f, 50.0f, 0.0f);
}

Shadow::~Shadow()
{
	glDeleteFramebuffersEXT(1, &fboID);
}

void Shadow::getDepthMap()
{
	shader.useProgram();
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fboID);
	glViewport(0, 0, width, height);
	glCullFace(GL_FRONT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Shadow::getDepthMapEND()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glCullFace(GL_BACK);
	int w, h;
	glfwGetFramebufferSize(window, &w, &h);
	glViewport(0, 0, w, h);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(0);
}

void Shadow::bind()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, shadowID);
}

glm::mat4 Shadow::getDepthVP(const Lights& lights, const glm::vec3& npos, bool flag)
{
	glm::vec3 pos;
	if (flag)
		pos = npos;
	else 
		pos = glm::vec3(lights.directionLight->getDirection());
	glm::mat4 lightView = glm::lookAt(pos, target, glm::vec3(0.0f, -1.0f, 0.0f));
	float virtualRight = width / height;
	//glm::mat4 lightProjection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, 2.0f, 200.0f);
	glm::mat4 lightProjection = glm::perspective(90.0f, virtualRight, 2.0f, 200.0f);

	return lightProjection * lightView;
}

/*
glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);


glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

glCullFace(GL_BACK);//剔除背面

glEnable(GL_TEXTURE_GEN_S);
glEnable(GL_TEXTURE_GEN_T);
glEnable(GL_TEXTURE_GEN_R);
glEnable(GL_TEXTURE_GEN_Q);
glEnable(GL_COLOR_MATERIAL);//激活颜色材料模式
glEnable(GL_CULL_FACE);//激活表面剔除*/