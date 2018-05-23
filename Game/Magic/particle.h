#pragma once
//
//  Particle.h
//  opengl
//
//  Created by app05 on 15-5-20.
//  Copyright (c) 2015�� app05. All rights reserved.

//˵�������Ǹ������ӽṹ����������õ���Ӧ���Ե�ֵ�������ֻ�Ǹ������������ԣ�

#ifndef __opengl__Particle__
#define __opengl__Particle__

#include "../shader.h"
#include "../camera.h"
#include "../texture.h"
#include "../stdh.h"
#include "../object.h"
#include "../renderer.h"
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>    

/** ���ӽṹ */
class Particle : public Model
{
public:
	float x, y, z;            /**< ���ӵ�λ�� */
	float radius;
	float tmpradius;
	float theta, phi;
	float vtheta, vphi;
	float atheta, aphi;
	unsigned int  red, gre, blu;    /**< ���ӵ���ɫ */
	float dx, dy, dz;
	float vx, vy, vz;         /**< ���ӵ��ٶ�(x,y,z����) */
	float ax, ay, az;         /**< ������x��y��z�ϵļ��ٶ� */
	float lifetime;         /**< ��������ֵ */
	float parsize;             /**< ���ӳߴ� */
	float dec;              /**< ������ʧ���ٶ� */

	void update(int ready, int explode);

	bool loadModel(const std::string& filePath);
	void draw() const;
	void drawDepthMap() const;

	void setVboData(const std::vector<Vertex>& vertData, const std::vector<GLuint>& indices);
	bool loadTexture(const std::string& filePath);

	GLuint VAOId, EBOId, vertexVBO;
	int size;
	Texture texture;
};

/** ������ */
class CParticle
{
public:
	vector<Particle> data;               /**< ����ָ�� */
	int         numparticle;        /**< ������Ŀ */
	int			lifetime;
	int			readytime;
	int			ready;
	int			explode;

	bool		isAttack;
	int flag;

public:
	CParticle();          /**< ���캯�� */
	~CParticle();         /**< �������� */

						  /** ������������ */
	int Create(long num);

	void InitFlame(float posx, float posy, float posz, float camx, float camy, float radius, Renderer& r);
	void Update();
	void Explodeinit();

	vector<Object*> particle;
};

#endif /* defined(__opengl__Particle__) */
