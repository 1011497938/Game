#include "model.h"
#include "../object.h"
#include <iostream>

void Model::setBaseUniform(const Shader& shader, const Camera& camera, const Object& object)
{
	glUniform1i(glGetUniformLocation(shader.getID(), "isAnim"), 0);
	glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "DepthBiasMVP"), 1, GL_FALSE, glm::value_ptr(object.depthBiasMVP));
	glm::mat4 modelView = camera.getViewMatrix() * object.model;
	glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "MV"), 1, GL_FALSE, glm::value_ptr(modelView));
	glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelView)));//
	glUniformMatrix3fv(glGetUniformLocation(shader.getID(), "NormalMatrix"), 1, GL_FALSE, glm::value_ptr(glm::mat3(normalMatrix)));
	glm::mat4 MVP = camera.getProjectionMatrix() * modelView;
	glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
	glUniform3fv(glGetUniformLocation(shader.getID(), "Material.diffuse"), 1, glm::value_ptr(object.diffuse));//漫反射
	glUniform3fv(glGetUniformLocation(shader.getID(), "Material.specular"), 1, glm::value_ptr(object.specular));//镜面反射
	glUniform1f(glGetUniformLocation(shader.getID(), "Material.shininess"), object.shininess);//光亮度
}

void Model::setDepthUniform(const Shader& depthShader, const glm::mat4& depthVP, Object& object)
{
	glUniform1i(glGetUniformLocation(depthShader.getID(), "isAnim"), 0);
	glm::mat4 MVP = depthVP * object.model;
	glUniformMatrix4fv(glGetUniformLocation(depthShader.getID(), "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
	glm::mat4 biasMatrix;
	biasMatrix = glm::translate(biasMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
	biasMatrix = glm::scale(biasMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
	if(object.flag == 1)
		object.depthBiasMVP = biasMatrix * MVP;
	else
		object.depthBiasMVP2 = biasMatrix * MVP;
}