#pragma once
#include "stdh.h"
#include "glm\glm.hpp"
#include "shader.h"
#include <vector>

using namespace std;

class DirectionLight
{
public:
	DirectionLight(const glm::vec4& direction, const glm::vec3& intensity) :
		m_direction(direction), m_intensity(intensity) {}
	inline const glm::vec4& getDirection() const { return m_direction; }
	inline const glm::vec3& getIntensity() const { return m_intensity; }
private:
	glm::vec4 m_direction;
	glm::vec3 m_intensity;
};

class Lights
{
	const Shader& shader;

public:
	Lights(const Shader& shader);
	~Lights();
	void setDirectionLight(DirectionLight* light);
	void clear();

	DirectionLight* directionLight;	
};
