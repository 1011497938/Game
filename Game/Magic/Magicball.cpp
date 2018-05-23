#include "Magicball.h"
#include "../resource.h"
#include <iostream>

#ifndef PI
#define PI 3.1415926535897f
#endif

#ifndef DTOR
#define DTOR (PI/180.0f)
#endif

float radius = 50.0f;

bool Magicball::loadModel(const std::string& filePath) {
	vector<Vertex> vertex;
	vector<GLuint> indices;

	float dtheta, dphi, hTile, vTile;
	dtheta = 20;
	dphi = 10;
	hTile = 1;
	vTile = 1;

	int NumVertex = int((360 / dtheta)*(180 / dphi));
	vertex.resize(4 * NumVertex);
	indices.resize(6 * NumVertex);

	float vx, vy, vz, mag, vu, vv;

	int n = 0;

	for (int phi = 0;phi <= 180 - dphi;phi += dphi) {
		for (int theta = 0;theta <= 360 - dtheta;theta += dtheta) {

			// Calculate the vertex at phi, theta
			vx = xCenter + radius * sinf(phi*DTOR) * cosf(DTOR*theta);
			vy = yCenter + radius * sinf(phi*DTOR) * sinf(DTOR*theta);
			vz = zCenter + radius * cosf(phi*DTOR);
			vertex[n++].position = glm::vec3(vx, vy, vz);

			// Calculate the vertex at phi+dphi, theta
			vx = xCenter + radius * sinf((phi + dphi)*DTOR) * cosf(DTOR*theta);
			vy = yCenter + radius * sinf((phi + dphi)*DTOR) * sinf(DTOR*theta);
			vz = zCenter + radius * cosf((phi + dphi)*DTOR);
			vertex[n++].position = glm::vec3(vx, vy, vz);

			// Calculate the vertex at phi, theta+dtheta
			vx = xCenter + radius * sinf(phi*DTOR) * cosf(DTOR*(theta + dtheta));
			vy = yCenter + radius * sinf(phi*DTOR) * sinf(DTOR*(theta + dtheta));
			vz = zCenter + radius * cosf(phi*DTOR);
			vertex[n++].position = glm::vec3(vx, vy, vz);

			// Calculate the vertex at phi+dphi, theta+dtheta
			vx = xCenter + radius * sinf((phi + dphi)*DTOR) * cosf(DTOR*(theta + dtheta));
			vy = yCenter + radius * sinf((phi + dphi)*DTOR) * sinf(DTOR*(theta + dtheta));
			vz = zCenter + radius * cosf((phi + dphi)*DTOR);
			vertex[n++].position = glm::vec3(vx, vy, vz);

			//cout << vx << "\t" << vy << "\t" << vz << endl;

			for (int i = n - 4;i < n;i++) {
				vx = vertex[i].position.x - xCenter;
				vy = vertex[i].position.y - yCenter;
				vz = vertex[i].position.z - zCenter;

				mag = (float)sqrt(vx*vx + vy*vy + vz*vz);
				vx /= mag;
				vy /= mag;
				vz /= mag;
				vertex[i].normal = glm::vec3(vx, vy, vz);

				if (fabs(vx - 0) <= 0.0001 && vy < 0) {
					if (i == n - 4 || i == n - 3)
						vu = 0;
					else
						vu = 1;
					vv = vTile * (1 - (float)(asinf(vz) / PI) - 0.5f);
				}
				else {
					vu = hTile * ((float)(atan2(vx, vy) / (PI * 2)) + 0.5f);
					vv = vTile * (1 - (float)(asinf(vz) / PI) - 0.5f);
				}
				//cout << vu << "\t" << vv << endl;
				vertex[i].texCoords = glm::vec2(vu, vv);
			}
		}
	}

	for (int i = 0; i < NumVertex; i++)
	{
		indices[6 * i] = 4 * i;
		indices[6 * i + 1] = 4 * i + 2;
		indices[6 * i + 2] = 4 * i + 1;

		indices[6 * i + 3] = 4 * i + 1;
		indices[6 * i + 4] = 4 * i + 2;
		indices[6 * i + 5] = 4 * i + 3;
	}

	setVboData(vertex, indices);
	return true;
}
void Magicball::draw() const {
	texture.bind(Texture::TEXTURE_DIFFUSE);

	if (VAOId == 0 || EBOId == 0 || vertexVBO == 0)
		return;
	glBindVertexArray(this->VAOId);
	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
void Magicball::drawDepthMap() const {
	if (VAOId == 0 || EBOId == 0 || vertexVBO == 0)
		return;
	glBindVertexArray(this->VAOId);
	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
void Magicball::setVboData(const std::vector<Vertex>& vertData, const std::vector<GLuint>& indices) {
	size = indices.size();
	glGenVertexArrays(1, &this->VAOId);
	glGenBuffers(1, &this->vertexVBO);
	glGenBuffers(1, &this->EBOId);

	glBindVertexArray(this->VAOId);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertData.size(), &vertData[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GL_FLOAT)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBOId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.size(), &indices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
bool Magicball::loadTexture(const std::string& filePath) {
	Texture* tmp;
	tmp = &texture;
	tmp->load2DTexture(filePath.c_str());

	return true;
}

void Magicball::setdirection(const glm::vec3& positon, const glm::vec2& direction, Renderer& r) {
	Flame.isAttack = true;
	xCenter = positon.x;
	yCenter = positon.y;
	zCenter = positon.z;
	camx = direction.x;
	camy = direction.y;
	camz = 0.0f;

	float mag = sqrt(camx*camx + camy*camy + camz*camz);

	camx /= mag;
	camy /= mag;
	camz /= mag;

	loadModel(" ");	
	//loadTexture("skydome.jpg");
	cout << xCenter << " " << yCenter << " " << zCenter << endl;
	cout << camx << " " << camy << " " << camz << endl;

	mball = new Object(this);
	mball->setMaterial(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.8f, 0.8f, 0.8f), 50);
	mball->setModelMat(glm::vec3(xCenter, yCenter, zCenter));

	//r.addObject(mball);

	Flame.Create(500);
	Flame.explode = 0;
	Flame.lifetime = 40;
	Flame.readytime = 30;
	Flame.ready = 1;
	Flame.flag = 0;
	Flame.InitFlame(xCenter + 0.1*camx, yCenter + 0.1*camx, zCenter + 1.5, camx, camy, 0.25f, r);
}

void Magicball::flash(bool running) {
	if (running) {
		if (Flame.readytime-- >= 0) {
			Flame.Update();
			return;
		}
		Flame.ready = 0;

		if (!Flame.flag) {
			for (Object* o : Callback::boundList) {
				for (int i = 0; i < 100; i += 10)
					if (Flame.particle[i]->obb.isIntersect(o->obb)) {
						hittedObject = o;
						Flame.flag = 1;
						break;
					}
			}
			if (Flame.flag) {
				Flame.lifetime = 1;
				Flame.explode = 0;
			}
		}
		
		if (Flame.lifetime-- >= 0) {
			Flame.Update();
			if (Flame.explode) {
				mball->toHide();
			}
			else
				mball->setModelMat(glm::vec3(camx, camy, camz));
			return;
		}
	}
}