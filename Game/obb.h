#pragma once

#include "stdh.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

using namespace std;

class Obb {
public:
	Obb(const glm::vec3& pos_info, const glm::vec3& len_info) {
		center_o = pos_info;
		extents = len_info;
	}
	void rotate(const float& angle, const glm::vec3& v) {
		rotation = glm::mat3(glm::rotate(glm::mat4(rotation), angle, v));
	}
	void translate(const glm::vec3& pos) {
		//center_o += pos;
	}
	void scale(const glm::vec3& size) {
		extents *= size;
		//center *= size;
	}
	bool isIntersect(const Obb& box);

	glm::vec3 center_o, center, center_d;
	glm::vec3 extents;//half x,y,z 's length
	glm::mat3 rotation;
};