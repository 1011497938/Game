#pragma once

#include "Model/model.h"
#include "obb.h"

class Model;

class Object
{
public:
	Object(Model* m, const int& v = 0);// : realModel(m), obb(m->pos_info, m->len_info) {}
	void setMaterial(const glm::vec3& ambientANDdiffuse, const glm::vec3& specular, const GLfloat& shininess);
	void setModelMat(const glm::vec3& position = glm::vec3(0.0f), const glm::vec3& size = glm::vec3(1.0f), const GLfloat& rotate = 0.0f,
		const glm::vec3& normal = glm::vec3(0.0f, 0.0f, 1.0f));
	void draw(const Shader& shader, const Camera& camera) const;
	void drawDepthMap(const Shader& shader, const glm::mat4& depthVP, const int& flag = 1);
	inline void toShow() { show = true; }
	inline void toHide() { show = false; }
	inline bool getState() { return show; }
	inline void changeModel(Model* m) { realModel = m; }
	inline int getDepthView() { return depthView; }
	friend class Model;
	Obb obb;

	bool toDelete = false;
private:
	bool show = true;
	int depthView;
	int flag;
	Model* realModel;
	glm::mat4 model, depthBiasMVP, depthBiasMVP2;
	glm::vec3 diffuse, specular;
	GLfloat shininess;
};
