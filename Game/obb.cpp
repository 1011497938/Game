#include "obb.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

bool Obb::isIntersect(const Obb& box)
{
	glm::vec3 v = this->center_d - box.center_d;

	//Compute A's basis  
	glm::vec3 VAx = box.rotation * glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 VAy = box.rotation * glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 VAz = box.rotation * glm::vec3(0.0f, 0.0f, 1.0f);

	glm::vec3 VA[3];
	VA[0] = VAx;
	VA[1] = VAy;
	VA[2] = VAz;

	//Compute B's basis  
	glm::vec3 VBx = this->rotation * glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 VBy = this->rotation * glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 VBz = this->rotation * glm::vec3(0.0f, 0.0f, 1.0f);

	glm::vec3 VB[3];
	VB[0] = VBx;
	VB[1] = VBy;
	VB[2] = VBz;

	glm::vec3 T(glm::dot(v, VAx), glm::dot(v, VAy), glm::dot(v, VAz));

	float R[3][3];
	float FR[3][3];
	float ra, rb, t;

	for (int i = 0; i < 3; i++)
	{
		for (int k = 0; k < 3; k++)
		{
			R[i][k] = glm::dot(VA[i], VB[k]);
			FR[i][k] = 1e-6f + abs(R[i][k]);
		}
	}

	// A's basis vectors  
	for (int i = 0; i < 3; i++)
	{
		ra = box.extents[i];
		rb = this->extents[0] * FR[i][0] + this->extents[1] * FR[i][1] + this->extents[2] * FR[i][2];
		t = abs(T[i]);
		if (t > ra + rb) return false;
	}

	// B's basis vectors  
	for (int k = 0; k < 3; k++)
	{
		ra = box.extents[0] * FR[0][k] + box.extents[1] * FR[1][k] + box.extents[2] * FR[2][k];
		rb = this->extents[k];
		t = abs(T[0] * R[0][k] + T[1] * R[1][k] + T[2] * R[2][k]);
		if (t > ra + rb) return false;
	}

	//9 cross products  

	//L = A0 x B0  
	ra = box.extents[1] * FR[2][0] + box.extents[2] * FR[1][0];
	rb = this->extents[1] * FR[0][2] + this->extents[2] * FR[0][1];
	t = abs(T[2] * R[1][0] - T[1] * R[2][0]);
	if (t > ra + rb) return false;

	//L = A0 x B1  
	ra = box.extents[1] * FR[2][1] + box.extents[2] * FR[1][1];
	rb = this->extents[0] * FR[0][2] + this->extents[2] * FR[0][0];
	t = abs(T[2] * R[1][1] - T[1] * R[2][1]);
	if (t > ra + rb) return false;

	//L = A0 x B2  
	ra = box.extents[1] * FR[2][2] + box.extents[2] * FR[1][2];
	rb = this->extents[0] * FR[0][1] + this->extents[1] * FR[0][0];
	t = abs(T[2] * R[1][2] - T[1] * R[2][2]);
	if (t > ra + rb) return false;

	//L = A1 x B0  
	ra = box.extents[0] * FR[2][0] + box.extents[2] * FR[0][0];
	rb = this->extents[1] * FR[1][2] + this->extents[2] * FR[1][1];
	t = abs(T[0] * R[2][0] - T[2] * R[0][0]);
	if (t > ra + rb) return false;

	//L = A1 x B1  
	ra = box.extents[0] * FR[2][1] + box.extents[2] * FR[0][1];
	rb = this->extents[0] * FR[1][2] + this->extents[2] * FR[1][0];
	t = abs(T[0] * R[2][1] - T[2] * R[0][1]);
	if (t > ra + rb) return false;

	//L = A1 x B2  
	ra = box.extents[0] * FR[2][2] + box.extents[2] * FR[0][2];
	rb = this->extents[0] * FR[1][1] + this->extents[1] * FR[1][0];
	t = abs(T[0] * R[2][2] - T[2] * R[0][2]);
	if (t > ra + rb) return false;

	//L = A2 x B0  
	ra = box.extents[0] * FR[1][0] + box.extents[1] * FR[0][0];
	rb = this->extents[1] * FR[2][2] + this->extents[2] * FR[2][1];
	t = abs(T[1] * R[0][0] - T[0] * R[1][0]);
	if (t > ra + rb) return false;

	//L = A2 x B1  
	ra = box.extents[0] * FR[1][1] + box.extents[1] * FR[0][1];
	rb = this->extents[0] * FR[2][2] + this->extents[2] * FR[2][0];
	t = abs(T[1] * R[0][1] - T[0] * R[1][1]);
	if (t > ra + rb) return false;

	//L = A2 x B2  
	ra = box.extents[0] * FR[1][2] + box.extents[1] * FR[0][2];
	rb = this->extents[0] * FR[2][1] + this->extents[1] * FR[2][0];
	t = abs(T[1] * R[0][2] - T[0] * R[1][2]);
	if (t > ra + rb) return false;

	return true;
}