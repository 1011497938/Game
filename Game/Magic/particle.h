#pragma once
//
//  Particle.h
//  opengl
//
//  Created by app05 on 15-5-20.
//  Copyright (c) 2015年 app05. All rights reserved.

//说明：就是根据粒子结构给其设置与得到对应属性的值（这个类只是给粒子设置属性）

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

/** 粒子结构 */
class Particle : public Model
{
public:
	float x, y, z;            /**< 粒子的位置 */
	float radius;
	float tmpradius;
	float theta, phi;
	float vtheta, vphi;
	float atheta, aphi;
	unsigned int  red, gre, blu;    /**< 粒子的颜色 */
	float dx, dy, dz;
	float vx, vy, vz;         /**< 粒子的速度(x,y,z方向) */
	float ax, ay, az;         /**< 粒子在x，y，z上的加速度 */
	float lifetime;         /**< 粒子生命值 */
	float parsize;             /**< 粒子尺寸 */
	float dec;              /**< 粒子消失的速度 */

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

/** 粒子类 */
class CParticle
{
public:
	vector<Particle> data;               /**< 粒子指针 */
	int         numparticle;        /**< 粒子数目 */
	int			lifetime;
	int			readytime;
	int			ready;
	int			explode;

	bool		isAttack;
	int flag;

public:
	CParticle();          /**< 构造函数 */
	~CParticle();         /**< 析构函数 */

						  /** 创建粒子数组 */
	int Create(long num);

	void InitFlame(float posx, float posy, float posz, float camx, float camy, float radius, Renderer& r);
	void Update();
	void Explodeinit();

	vector<Object*> particle;
};

#endif /* defined(__opengl__Particle__) */
