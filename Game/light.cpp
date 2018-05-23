#include "light.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>

Lights::Lights(const Shader& shader) :shader(shader)
{

}

Lights::~Lights()
{
	delete directionLight;
}

void Lights::setDirectionLight(DirectionLight* light)
{
	directionLight = light;
	glUniform4fv(glGetUniformLocation(shader.getID(), "TheDirectionLight.direction"), 1, glm::value_ptr(light->getDirection()));
	glUniform3fv(glGetUniformLocation(shader.getID(), "TheDirectionLight.intensity"), 1, glm::value_ptr(light->getIntensity()));
}

void Lights::clear()
{
	delete directionLight;
}