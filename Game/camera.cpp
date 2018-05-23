#include "camera.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp" 
#include <iostream>
struct BlockViewProjection {
	int size[2], offset[2];
} blockVP;

Camera::Camera()
{
}

Camera::~Camera()
{
}

Camera& Camera::operator=(const Camera& c2)
{
	this->pos = c2.pos;
	this->target = c2.target;

	this->side = c2.side;
	this->forward = c2.forward;
	this->up = c2.up;
	this->view = c2.view;
	this->projection = c2.projection;
	this->aspect = c2.aspect;

	return *this;
}

void Camera::setViewMatrix(const glm::vec3& pos, const glm::vec3& target, const glm::vec3& up)
{
	this->pos = pos;
	this->target = target;

	this->forward = glm::normalize(glm::vec3(target.x - pos.x, target.y - pos.y, target.z - pos.z));
	this->up = glm::normalize(up);
	this->side = glm::normalize(glm::cross(forward, up));

	flashViewMatrix();
}

void Camera::flashViewMatrix()
{
	view = glm::mat4(
		side.x, up.x, -forward.x, 0.0f,
		side.y, up.y, -forward.y, 0.0f,
		side.z, up.z, -forward.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	view = glm::translate(view, -pos);
}

void Camera::setProjectionMatrix(const GLfloat& fovy, const GLfloat& aspect, const GLfloat& near, const GLfloat& far)
{
	this->projection = glm::perspective(fovy, aspect, near, far);
	this->aspect = aspect;
}

void Camera::move(const GLfloat& dx, const GLfloat& dy, const GLfloat& dz)
{
	pos[0] = pos[0] + dx*side.x + dy*up.x + -dz*forward.x;
	pos[1] = pos[1] + dx*side.y + dy*up.y + -dz*forward.y;
	pos[2] = pos[2] + dx*side.z + dy*up.z + -dz*forward.z;

	flashViewMatrix();
}

void Camera::rotateN(const GLfloat& angle)
{
	GLfloat cs = cos(angle*PI / 180);
	GLfloat sn = sin(angle*PI / 180);
	glm::vec3 t(side);
	glm::vec3 s(up);

	side.x = cs*t.x - sn*s.x;
	side.y = cs*t.y - sn*s.y;
	side.z = cs*t.z - sn*s.z;
	up.x = sn*t.x + cs*s.x;
	up.y = sn*t.y + cs*s.y;
	up.z = sn*t.z + cs*s.z;

	flashViewMatrix();
}

void Camera::rotateU(const GLfloat& angle)
{
	GLfloat cs = cos(angle*PI / 180);
	GLfloat sn = sin(angle*PI / 180);
	glm::vec3 t(up);
	glm::vec3 s(forward);

	up.x = cs*t.x - sn*s.x;
	up.y = cs*t.y - sn*s.y;
	up.z = cs*t.z - sn*s.z;
	forward.x = sn*t.x + cs*s.x;
	forward.y = sn*t.y + cs*s.y;
	forward.z = sn*t.z + cs*s.z;


	flashViewMatrix();
}

void Camera::rotateV(const GLfloat& angle)
{
	GLfloat cs = cos(angle*PI / 180);
	GLfloat sn = sin(angle*PI / 180);
	glm::vec3 t(forward);
	glm::vec3 s(side);

	forward.x = cs*t.x - sn*s.x;
	forward.y = cs*t.y - sn*s.y;
	forward.z = cs*t.z - sn*s.z;
	side.x = sn*t.x + cs*s.x;
	side.y = sn*t.y + cs*s.y;
	side.z = sn*t.z + cs*s.z;

	flashViewMatrix();
}

GLfloat Camera::getDist() const
{
	GLfloat dist = pow(pos.x, 2) + pow(pos.y, 2) + pow(pos.z, 2);
	return pow(dist, 0.5);
}

void Camera::rotateX(const GLfloat& angle)
{
	GLfloat d = getDist();
	GLint cnt = 100;
	GLfloat theta = angle / cnt;
	GLfloat slide_d = -2 * d * sin(theta*PI / 360);
	rotateV(-theta / 2);
	for (; cnt != 0; --cnt)
	{
		move(slide_d, 0, 0);
		rotateV(-theta);
	}
	rotateV(theta / 2);
}

void Camera::rotateY(const GLfloat& angle)
{
	GLfloat d = getDist();
	GLint cnt = 100;
	GLfloat theta = angle / cnt;
	GLfloat slide_d = 2 * d*sin(theta*PI / 360);
	rotateU(-theta / 2);
	for (; cnt != 0; --cnt)
	{
		move(0, slide_d, 0);
		rotateU(-theta);
	}
	rotateU(theta / 2);
}

void Camera::changeDist(const GLfloat& dn)
{
	move(0, 0, dn);
}
