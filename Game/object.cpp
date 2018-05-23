#include "object.h"

Object::Object(Model* m, const int& v) :realModel(m), obb(m->pos_info, glm::vec3(m->len_info.x*0.9, m->len_info.y, m->len_info.z*0.9)) { depthView = v; }

void Object::draw(const Shader& shader, const Camera& camera) const
{
	glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "Model"), 1, GL_FALSE, glm::value_ptr(model));
	realModel->setBaseUniform(shader, camera, *this);
	if (depthView) {
		glPolygonOffset(-1.0, -1.0);
	}
	else {
		glPolygonOffset(1.0, 1.0);
	}
	realModel->draw();
}

void Object::drawDepthMap(const Shader& shader, const glm::mat4& depthVP, const int& flag)
{
	this->flag = flag;
	realModel->setDepthUniform(shader, depthVP, *this);
	if (depthView) {
		glPolygonOffset(-1.0, -1.0);
		realModel->drawDepthMap();
	}
}

void Object::setMaterial(const glm::vec3& ambientANDdiffuse, const glm::vec3& specular, const GLfloat& shininess)
{
	this->diffuse = ambientANDdiffuse;
	this->specular = specular;
	this->shininess = shininess;
}

void Object::setModelMat(const glm::vec3& position, const glm::vec3& size, const GLfloat& rotate, const glm::vec3& normal)
{
	glm::mat4 temp;
	temp = glm::translate(temp, position);
	temp = glm::rotate(temp, rotate, normal);
	temp = glm::scale(temp, size);

	model = model * temp;

	glm::vec4 n = model * glm::vec4(obb.center_o, 1.0f);
	//obb.center = glm::vec3(n.x, n.y, n.z);
	glm::vec4 d = model*glm::vec4(obb.center_o.x, 0.0f, obb.center_o.z, 1.0f);
	obb.center_d = glm::vec3(d.x, d.y, d.z);
	obb.center = obb.center_d;// +obb.extents;
	//obb.center_d.x += obb.extents.x;
	obb.center_d.y += obb.extents.y;
	obb.scale(size);
	obb.rotate(rotate, normal);
}