#include "stdh.h"
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glfw3dll.lib")

#include "object.h"
#include "renderer.h"
#include "camera.h"
#include "light.h"
#include "callback.h"
#include "shadow.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <iostream>
#include <vector>
#include <time.h>
#include "obb.h"
#include "Ground/ground.h"
#include "skybox/skybox.h"
#include "Model/baseModel.h"
#include "resource.h"
#include "model/objModel.h"
#include "Ground\enviromentObj.h"
#include "Magic\MagicBall.h"

#pragma comment(lib, "assimp.lib")

using namespace std;

int main()
{
	int width = 1920, height = 1080;
	GLFWwindow* window;
	if (!glfwInit())
		return -1;
	window = glfwCreateWindow(width, height, "Game", glfwGetPrimaryMonitor(), NULL);
	//window = glfwCreateWindow(width, height, "Game", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental = GL_TRUE;
	glewInit();
	glGetError();

	glEnable(GL_POLYGON_OFFSET_FILL);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	Shader s;//shader for vertex
	try {
		s.createObject(GL_VERTEX_SHADER, string("Resource\\v.glsl"));
		s.createObject(GL_FRAGMENT_SHADER, string("Resource\\f.glsl"));
		s.createProgram();
	}
	catch (Error e) { cout << e.getInfo(); system("pause"); return 0; }
	s.useProgram();
	
	Camera c; 
	c.setProjectionMatrix(75, width*1.0/height, 0.1, 2000);
	c.setViewMatrix(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	Callback::setCamera(&c);

	Lights lights(s);
	//lights.setDirectionLight(new DirectionLight(glm::vec4(-10.0f, -10.0f, 30.0f, 1.0f), glm::vec3(1.0f)));
	lights.setDirectionLight(new DirectionLight(glm::vec4(80.0f, 80.0f, 800.0f, 1.0f), glm::vec3(1.0f)));
	
	Renderer r(c, s);

	//人物
	AnimModel am;
	am.loadModel("Resource\\model\\manw.fbx");
	AnimModel am2;
	am2.loadModel("Resource\\model\\manst.fbx");
	Object oam(&am, 1);
	oam.setModelMat(glm::vec3(175.2f, 375.2f, 3.8f));
	oam.setModelMat(glm::vec3(0.0f), glm::vec3(1.0f), 90.0*PI / 180, glm::vec3(1.0f, 0.0f, 0.0f));
	oam.setModelMat(glm::vec3(0.0f), glm::vec3(0.03f));
	oam.setMaterial(glm::vec3(1.0f), glm::vec3(0.5f), 5);
	oam.obb.center_o = glm::vec3(0.0f, 2.3f / 2, 0.0f);
	Callback::setObject(&oam);
	r.addObject(&oam);

	//地形包围盒
	BaseModel bm;
	stringstream cubes;
	cubes << "Cube " << 900 << " " << 900 << " " << 300;
	bm.loadModel(cubes.str());
	Object obm(&bm);
	obm.setModelMat(glm::vec3(obm.obb.extents.x, 0.0f, 0.0f));
	obm.setModelMat(glm::vec3(0.0f), glm::vec3(0.95f));
	obm.setMaterial(glm::vec3(0.9f, 0.5f, 0.3f), glm::vec3(0.8f), 50);
	Callback::setTerrain(&obm);

	//天空盒
	Skybox box(r);

	//地形
	Ground *gm = new Ground("Resource\\data\\300", 300, 1, 900);
	Object* gbm = new Object(gm);
	gbm->setModelMat(glm::vec3(000.0f, 0.0f, 0.0f), glm::vec3(1.0f), -0, glm::vec3(0.0f, 0.0f, 1.0f));
	gbm->setMaterial(glm::vec3(0.5f), glm::vec3(1.0f), 128);
	r.addObject(gbm);

	//草
	ObjModel grassModel;
	grassModel.loadModel("Resource\\model\\grass.obj");
	float grassStep = 0.3;
	for (int i = 0; i < 100; i++)
	{
		int up = 20;
		int down = -20;
		float x = 184 + (rand() % (up - down + 1) + down) / 3.0;
		float y = 382 + (rand() % (up - down + 1) + down) / 4.0;
		Object* grassObj = new Object(&grassModel);
		grassObj->setModelMat(glm::vec3(x, y, gm->getObjectHeight(x, y) - 0.05), glm::vec3(0.005f));
		grassObj->setModelMat(glm::vec3(0.0f), glm::vec3(1.0f), 90.0*PI / 180, glm::vec3(1.0f, 0.0f, 0.0f));
		grassObj->setModelMat(glm::vec3(0.0f), glm::vec3(1.0f), (rand() % (up - down + 1) + down) * 3 * PI / 180, glm::vec3(0.0f, 1.0f, 0.0f));
		grassObj->setMaterial(glm::vec3(0.3f), glm::vec3(0.5f), 5);
		r.addObject(grassObj);
	}
	for (int i = 0; i < 100; i++)
	{
		int up = 20;
		int down = -20;
		float x = 181 + (rand() % (up - down + 1) + down) / 3.0;
		float y = 378 + (rand() % (up - down + 1) + down) / 4.0;
		Object* oom = new Object(&grassModel);
		oom->setModelMat(glm::vec3(x, y, gm->getObjectHeight(x, y) - 0.05), glm::vec3(0.005f));
		oom->setModelMat(glm::vec3(0.0f), glm::vec3(1.0f), 90.0*PI / 180, glm::vec3(1.0f, 0.0f, 0.0f));
		oom->setModelMat(glm::vec3(0.0f), glm::vec3(1.0f), (rand() % (up - down + 1) + down) * 3 * PI / 180, glm::vec3(0.0f, 1.0f, 0.0f));
		oom->setMaterial(glm::vec3(0.3f), glm::vec3(0.5f), 5);
		r.addObject(oom);
	}
	for (int i = 0; i < 900; i++)
	{
		int up = 100;
		int down = -100;
		float x = 181 + (rand() % (up - down + 1) + down) / 3.0;
		float y = 378 + (rand() % (up - down + 1) + down) / 4.0;
		Object* oom = new Object(&grassModel);
		oom->setModelMat(glm::vec3(x, y, gm->getObjectHeight(x, y) - 0.05), glm::vec3(0.005f));
		oom->setModelMat(glm::vec3(0.0f), glm::vec3(1.0f), 90.0*PI / 180, glm::vec3(1.0f, 0.0f, 0.0f));
		oom->setModelMat(glm::vec3(0.0f), glm::vec3(1.0f), (rand() % (up - down + 1) + down) * 3 * PI / 180, glm::vec3(0.0f, 1.0f, 0.0f));
		oom->setMaterial(glm::vec3(0.3f), glm::vec3(0.5f), 5);
		r.addObject(oom);
	}

	//树
	ObjModel treeModel;
	treeModel.loadModel("Resource\\model\\tree.obj");

	Object treeObj1(&treeModel, 1);
	treeObj1.obb.extents.z = treeObj1.obb.extents.x;
	treeObj1.setModelMat(glm::vec3(186.313, 389.186, gm->getObjectHeight(186.313, 389.186) - 0.05), glm::vec3(0.001f));
	//treeObj1.setModelMat(glm::vec3(186.313, 389.186, 2.54237 - 0.05), glm::vec3(0.001f));
	treeObj1.setModelMat(glm::vec3(0.0f), glm::vec3(1.0f), 90.0*PI / 180, glm::vec3(1.0f, 0.0f, 0.0f));
	treeObj1.setMaterial(glm::vec3(0.5f), glm::vec3(0.5f), 5);


	Object treeObj2(&treeModel, 1);
	treeObj2.obb.extents.z = treeObj2.obb.extents.x;
	treeObj2.setModelMat(glm::vec3(182.112, 378.921, gm->getObjectHeight(182.112, 378.921) - 0.05), glm::vec3(0.001f));
	treeObj2.setModelMat(glm::vec3(0.0f), glm::vec3(1.0f), 90.0*PI / 180, glm::vec3(1.0f, 0.0f, 0.0f));
	treeObj2.setMaterial(glm::vec3(0.5f), glm::vec3(0.5f), 5);


	Object treeObj3(&treeModel, 1);
	treeObj3.obb.extents.z = treeObj2.obb.extents.x;
	treeObj3.setModelMat(glm::vec3(171.79, 383.894, gm->getObjectHeight(171.79, 383.894) - 0.05), glm::vec3(0.001f));
	treeObj3.setModelMat(glm::vec3(0.0f), glm::vec3(1.0f), 90.0*PI / 180, glm::vec3(1.0f, 0.0f, 0.0f));
	treeObj3.setMaterial(glm::vec3(0.5f), glm::vec3(0.5f), 5);

	r.addObject(&treeObj1);
	r.addObject(&treeObj2);
	r.addObject(&treeObj3);
	Callback::boundList.push_back(&treeObj1);
	Callback::boundList.push_back(&treeObj2);
	Callback::boundList.push_back(&treeObj3);

	//远距离的树
	for (int i = 0; i < 100; i++)
	{
		int up = 27;
		int down = 0;

		int r_x = (rand() % (up - down + 1) + down);
		int r_y = (rand() % (up - down + 1) + down);

		while (r_x * r_x + r_y * r_y < 64 * 9)
		{
			r_x = (rand() % (up - down + 1) + down);
			r_y = (rand() % (up - down + 1) + down);
		}
		int flag_x = rand() % 2 ? -1 : 1;

		float x = 181 + r_x * r_x * flag_x / 9;
		float y = 425 + r_y * r_y * -1 / 9;

		Object* oom = new Object(&treeModel, 1);
		oom->setModelMat(glm::vec3(x, y, gm->getObjectHeight(x, y) - 0.05), glm::vec3(0.001f));
		oom->setModelMat(glm::vec3(0.0f), glm::vec3(1.0f), 90.0*PI / 180, glm::vec3(1.0f, 0.0f, 0.0f));
		oom->setModelMat(glm::vec3(0.0f), glm::vec3(1.0f), (rand() % (up - down + 1) + down) * 3 * PI / 180, glm::vec3(0.0f, 1.0f, 0.0f));
		oom->setMaterial(glm::vec3(0.5f), glm::vec3(0.5f), 5);
		r.addObject(oom);
		//Callback::boundList.push_back(oom);
	}

	//房子
	ObjModel houseModel;
	houseModel.loadModel("Resource\\model\\house.obj");
	Object housObj(&houseModel, 1);
	housObj.setModelMat(glm::vec3(182.591, 393.527, gm->getObjectHeight(182.591, 393.527) - 0.23), glm::vec3(0.006f));
	housObj.setModelMat(glm::vec3(0.0f), glm::vec3(1.0f), 91.0*PI / 180, glm::vec3(1.0f, 0.0f, 0.0f));
	housObj.setModelMat(glm::vec3(0.0f), glm::vec3(1.0f), 2.0*PI / 180, glm::vec3(0.0f, 0.0f, 1.0f));
	housObj.setModelMat(glm::vec3(0.0f), glm::vec3(1.0f), 90.0*PI / 180, glm::vec3(0.0f, 1.0f, 0.0f));
	housObj.setMaterial(glm::vec3(0.7f), glm::vec3(0.5f), 5);
	r.addObject(&housObj);

	BaseModel hm1;
	hm1.loadModel("Cube 2 2 5");
	Object ohm1(&hm1);
	ohm1.setModelMat(glm::vec3(182.591, 398.0, gm->getObjectHeight(182.591, 393.527) + 2.5));
	ohm1.setMaterial(glm::vec3(0.9f, 0.5f, 0.3f), glm::vec3(0.8f), 50);
	BaseModel hm2;
	hm2.loadModel("Cube 1.1 4 5");
	Object ohm2(&hm2);
	ohm2.setModelMat(glm::vec3(182.4, 393.5, gm->getObjectHeight(182.591, 393.527) + 2.5));
	ohm2.setMaterial(glm::vec3(0.9f, 0.5f, 0.3f), glm::vec3(0.8f), 50);
	BaseModel hm3;
	hm3.loadModel("Cube 3 0.2 5");
	Object ohm3(&hm3);
	ohm3.setModelMat(glm::vec3(179.1, 393.1, gm->getObjectHeight(182.591, 393.527) + 2.5));
	ohm3.setMaterial(glm::vec3(0.9f, 0.5f, 0.3f), glm::vec3(0.8f), 50);
	BaseModel hm4;
	hm4.loadModel("Cube 0.4 7 5");
	Object ohm4(&hm4);
	ohm4.setModelMat(glm::vec3(177.6, 390.0, gm->getObjectHeight(182.591, 393.527) + 2.5), glm::vec3(1.0f), 30.0*PI / 180);
	ohm4.setMaterial(glm::vec3(0.9f, 0.5f, 0.3f), glm::vec3(0.8f), 50);
	BaseModel hm5;
	hm5.loadModel("Cube 7 0.8 5");
	Object ohm5(&hm5);
	ohm5.setModelMat(glm::vec3(178.2, 397.8, gm->getObjectHeight(182.591, 393.527) + 2.5), glm::vec3(1.0f), 20.0*PI / 180);
	ohm5.setMaterial(glm::vec3(0.9f, 0.5f, 0.3f), glm::vec3(0.8f), 50);
	
	Callback::boundList.push_back(&ohm1);
	Callback::boundList.push_back(&ohm2);
	Callback::boundList.push_back(&ohm3);
	Callback::boundList.push_back(&ohm4);
	Callback::boundList.push_back(&ohm5);

	Magicball* mb = new Magicball();

	EnviromentObj e(gm, &r);

	Shader dS; //shader for depth
	try {
		dS.createObject(GL_VERTEX_SHADER, string("Resource\\vDepth.glsl"));
		dS.createObject(GL_FRAGMENT_SHADER, string("Resource\\fDepth.glsl"));
		dS.createProgram();
	}
	catch (Error e) { cout << e.getInfo(); system("pause"); return 0; }
	Shadow shadow(window, dS, 10000, 10000);

	Callback::setGround(gm);
	Callback call(window);
	
	ftime(&Renderer::start);
	float t = 0, t1 = 6 - 4.0 / 30, t2 = 11, tn, tt, tt2;
	bool attacked = false, attacking = false;
	int treeObjRTimes = 0;
	while (!glfwWindowShouldClose(window))
	{
		e.draw(oam.obb.center.x, oam.obb.center.y);
		if (mb->hittedObject == &treeObj1)
		{
			if (treeObjRTimes < 90)
			{
				treeObj1.setModelMat(glm::vec3(0.0f), glm::vec3(1.0f), PI / 180, glm::vec3(1.0f, 0.0f, 0.0f));
				treeObjRTimes++;
			}
			else {
				mb->hittedObject = NULL;
				treeObjRTimes = 0;
			}
		}
		if (mb->hittedObject == &treeObj2)
		{
			if (treeObjRTimes < 90)
			{
				treeObj2.setModelMat(glm::vec3(0.0f), glm::vec3(1.0f), PI / 180, glm::vec3(1.0f, 0.0f, 0.0f));
				treeObjRTimes++;
			}
			else {
				mb->hittedObject = NULL;
				treeObjRTimes = 0;
			}
		}
		if (mb->hittedObject == &treeObj3)
		{
			if (treeObjRTimes < 90)
			{
				treeObj3.setModelMat(glm::vec3(0.0f), glm::vec3(1.0f), PI / 180, glm::vec3(1.0f, 0.0f, 0.0f));
				treeObjRTimes++;
			}
			else {
				mb->hittedObject = NULL;
				treeObjRTimes = 0;
			}
		}
		if (Callback::isAttack) {
			if(!mb->Flame.isAttack)
				mb->setdirection(oam.obb.center, Callback::forward, r);
		}
		if (Callback::isAttack)
		{
			attacked = true;
			tt = Renderer::getRunningTime() - tn;
			if (tt < 26.0 / 30) {
				am2.getBoneTransform(11 - tt*2);
			}
			else {
				attacking = true;
				am2.getBoneTransform(9 + 8 / 30);
			}
			oam.changeModel(&am2);
		}
		else {
			if (attacked) {
				tt2 = Renderer::getRunningTime() - tt - tn;
				t = Renderer::getRunningTime();
				if (tt2 > 26.0 / 30) {
					attacked = false;
				}
				else {
					attacking = false;
					am2.getBoneTransform(9 + 8 / 30 + 2*tt2);
				}
			}
			else {
				tn = Renderer::getRunningTime();
				if (Callback::isWalking) {
					t = Renderer::getRunningTime();
					if (Callback::dx > 0)
						am.getBoneTransform(t);
					else if(Callback::dx <0)
						am.getBoneTransform((44.0 - (int)(t*30)%44)/30);
					oam.changeModel(&am);
				}
				else {
					am2.getBoneTransform(Renderer::getRunningTime() - t);
					oam.changeModel(&am2);
				}
			}
		}
		
		shadow.setTarget(glm::vec3(oam.obb.center.x, oam.obb.center.y, 0.0f));
		//glm::vec3 npos = glm::vec3(-5.0f, -5.0f, 15.0f);
		glm::vec3 npos = glm::vec3(155.0f, 355.0f, 20.0f);
		glm::mat4 depthVP = shadow.getDepthVP(lights, npos, true);// +oam.obb.center, true);
		shadow.getDepthMap();
		r.renderDepthMap(depthVP, shadow.getShader());
		shadow.getDepthMapEND();
		

		//draw
		shadow.bind();
		glCullFace(GL_BACK);
		r.setCenterPos(oam.obb.center);
		r.render();
		
		if(mb->Flame.isAttack)
			mb->flash(1);
		Callback::moving();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}