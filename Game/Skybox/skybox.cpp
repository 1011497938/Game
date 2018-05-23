#include "skybox.h"
#include "../resource.h"
#include <iostream>

const int length = 1800;
const int height = 900;
const int x = -450;
const int y = -450;
const int z = -500;

void SkyboxTop::setVboData(const std::vector<Vertex>& vertData, const std::vector<GLuint>& indices){
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

void SkyboxTop::drawDepthMap() const
{
	if (VAOId == 0 || EBOId == 0 || vertexVBO == 0)
		return;
	glBindVertexArray(this->VAOId);
	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

bool SkyboxTop::loadModel(const std::string& filePath) {
	vector<Vertex> vertex;
	vector<GLuint> indices;
	vertex.resize(4);
	indices.resize(6);

	vertex[0].position = glm::vec3(x, y, z + height);
	vertex[1].position = glm::vec3(x, y + length, z + height);
	vertex[2].position = glm::vec3(x + length, y, z + height);
	vertex[3].position = glm::vec3(x + length, y + length, z + height);

	indices[0] = 0; indices[1] = 2; indices[2] = 3;
	indices[3] = 0; indices[4] = 3; indices[5] = 1;

	for (int i = 0; i < vertex.size(); i++)
	{
		float vx = vertex[i].position.x - x - length / 2;
		float vy = vertex[i].position.y - y - length / 2;
		float vz = vertex[i].position.z - z - height / 2;
		float mag = sqrt(vx*vx + vy*vy + vz*vz);
		vx /= mag;	vy /= mag;	vz /= mag;
		vertex[i].normal = glm::vec3(vx, vy, vz);
		vertex[i].texCoords = glm::vec2((vertex[i].position.y - y) / length, (vertex[i].position.x - x) / length);
	}
	setVboData(vertex, indices);
	return true;
}

void SkyboxTop::draw() const
{
	texture.bind(Texture::TEXTURE_DIFFUSE);

	if (VAOId == 0 || EBOId == 0 || vertexVBO == 0)
		return;
	glBindVertexArray(this->VAOId);
	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

bool SkyboxTop::loadTexture(const std::string& filePath) {
	Texture* tmp;
	tmp = &texture;
	tmp->load2DTexture(filePath.c_str());

	return true;
}

void SkyboxBack::setVboData(const std::vector<Vertex>& vertData, const std::vector<GLuint>& indices) {
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

void SkyboxBack::drawDepthMap() const
{
	if (VAOId == 0 || EBOId == 0 || vertexVBO == 0)
		return;
	glBindVertexArray(this->VAOId);
	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

bool SkyboxBack::loadModel(const std::string& filePath) {
	vector<Vertex> vertex;
	vector<GLuint> indices;
	vertex.resize(4);
	indices.resize(6);

	vertex[0].position = glm::vec3(x, y, z);
	vertex[1].position = glm::vec3(x, y, z + height);
	vertex[2].position = glm::vec3(x, y + length, z);
	vertex[3].position = glm::vec3(x, y + length, z + height);

	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 1; indices[4] = 3; indices[5] = 2;

	for (int i = 0; i < vertex.size(); i++)
	{
		float vx = vertex[i].position.x - x - length / 2;
		float vy = vertex[i].position.y - y - length / 2;
		float vz = vertex[i].position.z - z - height / 2;
		float mag = sqrt(vx*vx + vy*vy + vz*vz);
		vx /= mag;	vy /= mag;	vz /= mag;
		vertex[i].normal = glm::vec3(vx, vy, vz);
		vertex[i].texCoords = glm::vec2(1 - (vertex[i].position.y - y) / length, 1 - (vertex[i].position.z - z) / height);
	}

	setVboData(vertex, indices);

	return true;
}

void SkyboxBack::draw() const
{
	texture.bind(Texture::TEXTURE_DIFFUSE);

	if (VAOId == 0 || EBOId == 0 || vertexVBO == 0)
		return;
	glBindVertexArray(this->VAOId);
	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

bool SkyboxBack::loadTexture(const std::string& filePath) {
	Texture* tmp;
	tmp = &texture;
	tmp->load2DTexture(filePath.c_str());

	return true;
}

void SkyboxFront::setVboData(const std::vector<Vertex>& vertData, const std::vector<GLuint>& indices) {
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

void SkyboxFront::drawDepthMap() const
{
	if (VAOId == 0 || EBOId == 0 || vertexVBO == 0)
		return;
	glBindVertexArray(this->VAOId);
	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

bool SkyboxFront::loadModel(const std::string& filePath) {
	vector<Vertex> vertex;
	vector<GLuint> indices;
	vertex.resize(4);
	indices.resize(6);

	vertex[0].position = glm::vec3(x + length, y, z);
	vertex[1].position = glm::vec3(x + length, y, z + height);
	vertex[2].position = glm::vec3(x + length, y + length, z);
	vertex[3].position = glm::vec3(x + length, y + length, z + height);

	indices[0] = 0; indices[1] = 2; indices[2] = 3;
	indices[3] = 0; indices[4] = 3; indices[5] = 1;

	for (int i = 0; i < vertex.size(); i++)
	{
		float vx = vertex[i].position.x - x - length / 2;
		float vy = vertex[i].position.y - y - length / 2;
		float vz = vertex[i].position.z - z - height / 2;
		float mag = sqrt(vx*vx + vy*vy + vz*vz);
		vx /= mag;	vy /= mag;	vz /= mag;
		vertex[i].normal = glm::vec3(vx, vy, vz);
		vertex[i].texCoords = glm::vec2((vertex[i].position.y - y) / length, 1 - (vertex[i].position.z - z) / height);
	}

	setVboData(vertex, indices);

	return true;
}

void SkyboxFront::draw() const
{
	texture.bind(Texture::TEXTURE_DIFFUSE);

	if (VAOId == 0 || EBOId == 0 || vertexVBO == 0)
		return;
	glBindVertexArray(this->VAOId);
	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

bool SkyboxFront::loadTexture(const std::string& filePath) {
	Texture* tmp;
	tmp = &texture;
	tmp->load2DTexture(filePath.c_str());

	return true;
}

void SkyboxLeft::setVboData(const std::vector<Vertex>& vertData, const std::vector<GLuint>& indices) {
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

void SkyboxLeft::drawDepthMap() const
{
	if (VAOId == 0 || EBOId == 0 || vertexVBO == 0)
		return;
	glBindVertexArray(this->VAOId);
	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

bool SkyboxLeft::loadModel(const std::string& filePath) {
	vector<Vertex> vertex;
	vector<GLuint> indices;
	vertex.resize(4);
	indices.resize(6);

	vertex[0].position = glm::vec3(x, y, z);
	vertex[1].position = glm::vec3(x + length, y, z);
	vertex[2].position = glm::vec3(x, y, z + height);
	vertex[3].position = glm::vec3(x + length, y, z + height);

	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 1; indices[4] = 3; indices[5] = 2;

	for (int i = 0; i < vertex.size(); i++)
	{
		float vx = vertex[i].position.x - x - length / 2;
		float vy = vertex[i].position.y - y - length / 2;
		float vz = vertex[i].position.z - z - height / 2;
		float mag = sqrt(vx*vx + vy*vy + vz*vz);
		vx /= mag;	vy /= mag;	vz /= mag;
		vertex[i].normal = glm::vec3(vx, vy, vz);
		vertex[i].texCoords = glm::vec2((vertex[i].position.x - x) / length, 1 - (vertex[i].position.z - z) / height);
	}

	setVboData(vertex, indices);

	return true;
}

void SkyboxLeft::draw() const
{
	texture.bind(Texture::TEXTURE_DIFFUSE);

	if (VAOId == 0 || EBOId == 0 || vertexVBO == 0)
		return;
	glBindVertexArray(this->VAOId);
	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

bool SkyboxLeft::loadTexture(const std::string& filePath) {
	Texture* tmp;
	tmp = &texture;
	tmp->load2DTexture(filePath.c_str());

	return true;
}

void SkyboxRight::setVboData(const std::vector<Vertex>& vertData, const std::vector<GLuint>& indices) {
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

void SkyboxRight::drawDepthMap() const
{
	if (VAOId == 0 || EBOId == 0 || vertexVBO == 0)
		return;
	glBindVertexArray(this->VAOId);
	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

bool SkyboxRight::loadModel(const std::string& filePath) {
	vector<Vertex> vertex;
	vector<GLuint> indices;
	vertex.resize(4);
	indices.resize(6);

	vertex[0].position = glm::vec3(x, y + length, z);
	vertex[1].position = glm::vec3(x + length, y + length, z);
	vertex[2].position = glm::vec3(x, y + length, z + height);
	vertex[3].position = glm::vec3(x + length, y + length, z + height);

	indices[0] = 0; indices[1] = 2; indices[2] = 1;
	indices[3] = 1; indices[4] = 2; indices[5] = 3;

	for (int i = 0; i < vertex.size(); i++)
	{
		float vx = vertex[i].position.x - x - length / 2;
		float vy = vertex[i].position.y - y - length / 2;
		float vz = vertex[i].position.z - z - height / 2;
		float mag = sqrt(vx*vx + vy*vy + vz*vz);
		vx /= mag;	vy /= mag;	vz /= mag;
		vertex[i].normal = glm::vec3(vx, vy, vz);
		vertex[i].texCoords = glm::vec2(1 - (vertex[i].position.x - x) / length, 1 - (vertex[i].position.z - z) / height);
	}

	setVboData(vertex, indices);

	return true;
}

void SkyboxRight::draw() const
{
	texture.bind(Texture::TEXTURE_DIFFUSE);

	if (VAOId == 0 || EBOId == 0 || vertexVBO == 0)
		return;
	glBindVertexArray(this->VAOId);
	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

bool SkyboxRight::loadTexture(const std::string& filePath) {
	Texture* tmp;
	tmp = &texture;
	tmp->load2DTexture(filePath.c_str());

	return true;
}