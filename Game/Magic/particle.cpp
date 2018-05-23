#include "Particle.h"

bool Particle::loadModel(const std::string& filePath) {
	float l = parsize, w = parsize, h = parsize;

	vector<Vertex> vertex;
	vector<GLuint> indices;
	vertex.resize(24);
	indices.resize(36);
	len_info = glm::vec3(l / 2, w / 2, h / 2);

	for (int i = 0; i < 8; i++)
		vertex[i].texCoords = glm::vec2(0.5f);

	vector<glm::vec3> normal;
	normal.resize(6);

	{
		vertex[0].position = glm::vec3(-l / 2, -w / 2, h / 2);
		vertex[1].position = glm::vec3(-l / 2, w / 2, h / 2);
		vertex[2].position = glm::vec3(l / 2, w / 2, h / 2);
		vertex[3].position = glm::vec3(l / 2, -w / 2, h / 2);

		vertex[7].position = glm::vec3(-l / 2, -w / 2, -h / 2);
		vertex[6].position = glm::vec3(-l / 2, w / 2, -h / 2);
		vertex[5].position = glm::vec3(l / 2, w / 2, -h / 2);
		vertex[4].position = glm::vec3(l / 2, -w / 2, -h / 2);

		vertex[8].position = vertex[2].position;
		vertex[9].position = vertex[1].position;
		vertex[10].position = vertex[6].position;
		vertex[11].position = vertex[5].position;

		vertex[12].position = vertex[3].position;
		vertex[13].position = vertex[2].position;
		vertex[14].position = vertex[5].position;
		vertex[15].position = vertex[4].position;

		vertex[16].position = vertex[0].position;
		vertex[17].position = vertex[3].position;
		vertex[18].position = vertex[4].position;
		vertex[19].position = vertex[7].position;

		vertex[20].position = vertex[1].position;
		vertex[21].position = vertex[0].position;
		vertex[22].position = vertex[7].position;
		vertex[23].position = vertex[6].position;

		normal[0] = glm::vec3(0.0f, 0.0f, 1.0f);
		normal[1] = glm::vec3(0.0f, 0.0f, -1.0f);
		normal[2] = glm::vec3(0.0f, 1.0f, 0.0f);
		normal[3] = glm::vec3(1.0f, 0.0f, 0.0f);
		normal[4] = glm::vec3(0.0f, -1.0f, 0.0f);
		normal[5] = glm::vec3(-1.0f, 0.0f, 0.0f);
	}

	for (int i = 0; i < 6; i++)
	{
		indices[i * 6 + 0] = i * 4 + 0;
		indices[i * 6 + 1] = i * 4 + 1;
		indices[i * 6 + 2] = i * 4 + 2;
		indices[i * 6 + 3] = i * 4 + 0;
		indices[i * 6 + 4] = i * 4 + 2;
		indices[i * 6 + 5] = i * 4 + 3;
		for (int j = 0; j < 4; j++) {
			vertex[i * 4 + j].normal = normal[i];
			vertex[i * 4 + j].texCoords = glm::vec2(vertex[i * 4 + j].position.x, vertex[i * 4 + j].position.y);
		}
	}
	setVboData(vertex, indices);
	return true;
}
void Particle::draw() const {
	texture.bind(Texture::TEXTURE_DIFFUSE);

	if (VAOId == 0 || EBOId == 0 || vertexVBO == 0)
		return;
	glBindVertexArray(this->VAOId);
	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
void Particle::drawDepthMap() const {
	if (VAOId == 0 || EBOId == 0 || vertexVBO == 0)
		return;
	glBindVertexArray(this->VAOId);
	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
void Particle::setVboData(const std::vector<Vertex>& vertData, const std::vector<GLuint>& indices) {
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
bool Particle::loadTexture(const std::string& filePath) {
	Texture* tmp;
	tmp = &texture;
	tmp->load2DTexture(filePath.c_str());
	return true;
}

/** 构造函数 */
CParticle::CParticle()
{
	numparticle = 0;
}

/** 析构函数 */
CParticle::~CParticle()
{
	data.resize(0);
}

/** 创建一个包含num个元素的粒子数组 */
int CParticle::Create(long num)
{
	numparticle = num;
	data.resize(numparticle);
	
	return numparticle;
}

/** 初始化粒子 */
void CParticle::InitFlame(float posx, float posy, float posz, float camx, float camy, float radius, Renderer& r)
{
	particle.resize(numparticle);
	for (int i = 0; i < numparticle; ++i)
	{
		///初始化极坐标
		data[i].theta = 0.3f * (rand() % 1000);
		data[i].phi = 0.3f*(rand() % 1000);

		data[i].vtheta = 0.01f*(rand() % 100 - rand() % 100);
		data[i].vphi = 0.01f*(rand() % 100 - rand() % 100);

		data[i].atheta = 0.01f*(rand() % 100 - rand() % 100);
		data[i].aphi = 0.01f*(rand() % 100 - rand() % 100);

		///初始化直角坐标
		data[i].dx = posx;
		data[i].dy = posy;
		data[i].dz = posz;

		data[i].vx = camx;
		data[i].vy = camy;
		data[i].vz = 0;

		///初始化大小
		data[i].parsize = 0.15f;

		data[i].radius = radius * 8;

		data[i].tmpradius = 0.002f * (rand() % 50 - rand() % 50);

		data[i].x = data[i].dx + ((data[i].radius + data[i].tmpradius) * sinf(data[i].theta) * sinf(data[i].phi));
		data[i].y = data[i].dy + ((data[i].radius + data[i].tmpradius) * cosf(data[i].theta) * sinf(data[i].phi));
		data[i].z = data[i].dz + ((data[i].radius + data[i].tmpradius) * cosf(data[i].phi));

		if (i == 0) {
			data[0].loadModel(" ");
			//data[0].loadTexture("white.jpg");
		}

		particle[i] = new Object(&data[0]);

		float red = 0.1f*(rand() % 10);
		float gre = 159 / 256.0;//0.1f*(rand() % 10);
		float blu = 0.1f*(rand() % 10);

		particle[i]->setMaterial(glm::vec3(red, gre, blu), glm::vec3(1.0f), 50);

		particle[i]->setModelMat(glm::vec3(data[i].x, data[i].y, data[i].z));

		r.addObject(particle[i]);
	}
}

void Particle::update(int ready, int explode) {
	if (ready) {
		x = 0 - (radius + tmpradius) * sinf(theta) * sinf(phi);
		y = 0 - (radius + tmpradius) * cosf(theta) * sinf(phi);
		z = 0 - (radius + tmpradius) * cosf(phi);

		vtheta += atheta;
		vphi += aphi;

		theta += vtheta;
		phi += vphi;

		radius *= 0.933;

		tmpradius = 0.002f * (rand() % 50 - rand() % 50);

		x += (radius + tmpradius) * sinf(theta) * sinf(phi);
		y += (radius + tmpradius) * cosf(theta) * sinf(phi);
		z += (radius + tmpradius) * cosf(phi);
	}
	else if (!explode) {
		/** 更新位置 */
		x = 0 - (radius + tmpradius) * sinf(theta) * sinf(phi);
		y = 0 - (radius + tmpradius) * cosf(theta) * sinf(phi);
		z = 0 - (radius + tmpradius) * cosf(phi);

		x += vx;
		y += vy;
		z += vz;

		vtheta += atheta;
		vphi += aphi;

		theta += vtheta;
		phi += vphi;

		tmpradius = 0.002f * (rand() % 50 - rand() % 50);

		x += (radius + tmpradius) * sinf(theta) * sinf(phi);
		y += (radius + tmpradius) * cosf(theta) * sinf(phi);
		z += (radius + tmpradius) * cosf(phi);
	}
	else {
		x = vx;
		y = vy;
		z = vz;

		vx += ax;
		vy += ay;
		vz += az;
	}
}

void CParticle::Explodeinit() {
	lifetime = 30;
	for (int i = 0; i < numparticle; ++i)
	{
		data[i].vx = 0.001f*(rand() % 100 - rand() % 100);
		data[i].vy = 0.001f*(rand() % 100 - rand() % 100);
		data[i].vz = 0.001f*(rand() % 100 - rand() % 100);

		data[i].ax = 0.001f*(rand() % 100 - rand() % 100);
		data[i].ay = 0.001f*(rand() % 100 - rand() % 100);
		data[i].az = 0.001f*(rand() % 100 - rand() % 100);
	}
}


void CParticle::Update() {
	for (int i = 0;i < numparticle;i++) {
		data[i].update(ready, explode);
		particle[i]->setModelMat(glm::vec3(data[i].x, data[i].y, data[i].z));
	}
	if (lifetime <= 0) {
		if (explode) {
			for (int i = 0;i < numparticle;i++) {
				particle[i]->toHide();
			}
			isAttack = false;
		}
		else {
			Explodeinit();
			explode = 1;
		}
	}
}