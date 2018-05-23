#pragma once

#include "stdh.h"
#include "glm/glm.hpp"
#include "shader.h"

const double PI = 3.14159265;

class Camera
{
public:
	Camera();
	Camera& operator=(const Camera& c2);
	~Camera();
	void setViewMatrix(const glm::vec3& pos, const glm::vec3& target, const glm::vec3& up);
	void flashViewMatrix();
	void setProjectionMatrix(const GLfloat& fovy, const GLfloat& aspect, const GLfloat& near, const GLfloat& far);
	
	void rotateX(const GLfloat& angle);
	void rotateY(const GLfloat& angle);
	void changeDist(const GLfloat& dn);

	void move(const GLfloat& dx, const GLfloat& dy, const GLfloat& dz);
	void rotateN(const GLfloat& angle);
	void rotateU(const GLfloat& angle);
	void rotateV(const GLfloat& angle);
	
	GLfloat getDist() const;
	inline const GLfloat& getAspect() const { return aspect; }
	inline const glm::vec3& getPos() const { return pos; }
	inline const glm::vec3& getTarget() const { return target; }
	inline const glm::vec3& getUp() const { return up; }
	inline const glm::mat4& getViewMatrix() const { return view; }
	inline const glm::mat4& getProjectionMatrix() const { return projection; }

private:
	glm::mat4 projection;
	GLfloat aspect;

	glm::vec3 pos, target;
	glm::vec3 forward, up, side;
	glm::mat4 view;
};
