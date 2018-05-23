#include "ground.h"
#include <fstream>
#include <iostream>
#include "../renderer.h"
#include "../shadow.h"
#include <vector>
#include <time.h>    
#include <math.h>    

class  EnviromentObj
{
public:
	 EnviromentObj(Ground *_ground, Renderer *_r);
	void draw(int positionX, int positionY);
	vector<Object*> obj;
	vector<vec2> objPosition;
	float showDist = 100;
private:
	float step;
	vector<vector<float>>* data;
	int n;
	vector<vector<kind>> envObjData; //花花草草的数据
	int minHeight;
	int maxHeight;
	int random(int m, int n) {
		return rand() % (n - m + 1) + m;
	}
	Renderer *r;
	Ground *ground;

	ObjModel grassModel;
	ObjModel treeModel;
};

 EnviromentObj::EnviromentObj(Ground *_ground, Renderer *_r)
{
	 treeModel.loadModel("Resource\\model\\tree.obj");
	 //grassModel.loadModel("Resource\\model\\grass.obj");

	 ground = _ground;
	 data = &(ground->data);
	 n = data->size();
	 r = _r;
	 step = ground->step;

	 minHeight = 9999;
	 maxHeight = -9999;
	 for (int i = 0; i < n; i++)
		 for (int j = 0; j < n; j++) {
			 if (minHeight > (*data)[i][j])
				 minHeight = (*data)[i][j];
			 if (maxHeight <(*data)[i][j])
				 maxHeight = (*data)[i][j];
		 }
	 
	 
	 envObjData.resize(n);
	 for (int i = 0; i < n; i++){
		 envObjData[i].resize(n);
		 for (int j = 0; j < n; j++)
			 envObjData[i][j] = 0;
		 
	 }

	 int envObjDense = ground->n * ground->n / 100;
	 //envObjDense = 0;
	 int num = 0;
	 while (num<envObjDense)
	 {
		 try
		 {
			 int startX = random(n / 5, n * 4 / 5);
			 int startY = random(n / 5, n * 4 / 5);

			 if ((*data)[startX][startY] > random(minHeight, maxHeight) / 2)
				 continue;

			 kind k = random(1, 40);
			 if (k > 20)
				 k = 2;

			 envObjData[startX][startY] = k;
			 num++;
			 int r = random(20, 40);
			 while (random(0, r) < r)
			 {
				 for (int i = -1; i <= 1; i++)
					 for (int j = -1; j <= 1; j++) {
						 if ((*data)[startX + i][startY + j] > random(minHeight, maxHeight) / 2)
							 continue;
						 kind k = random(1, 40);
						 if (k > 20)
							 k = 2;
						 envObjData[startX + random(6, 20)][startY + random(6, 20)] = k;
						 num++;
					 }

				 kind k = random(1, 40);
				 if (k > 20)
					 k = 2;

				 try{
					 int r1 = random(-12, 12);
					 int r2 = random(-12, 12);
					 while ((*data)[startX + r1][startY + r2] > random(minHeight, maxHeight)) {
						 r1 = random(-12, 12);
						 r2 = random(-12, 12);
					 }
					 startX += r1;
					 startY += r2;
				 }
				 catch (const std::exception&)
				 {

				 }


				 if (startX < 0 || startY < 0 | startX >= n || startY >= n)
					 break;
				 envObjData[startX][startY] = k;
			 }
		 }
		 catch (const std::exception&)
		 {

		 }

	 }

	 for (int i = 179/step - 100; i < 179/step + 100; i++) {
	 	for (int j = 378 / step - 100; j < 378 / step + 100; j++) {
			if (i > 0 && j > 0)
				envObjData[i][j] = 0;
	 	}
	 }

	 //for (int i = 0; i < (t->obj).size(); i++)
		// obj.push_back((t->obj)[i]->obj);
	 //for (int i = 0; i < (g->obj).size(); i++)
		// obj.push_back((g->obj)[i]->obj);

	 cout << "生成静态物体" << endl;
}



 void  EnviromentObj::draw(int positionX, int positionY) {

	 static int preX = -100;
	 static int preY = -100;
	 if (preX == positionX && preY == positionY)
		 return;

	 try
	 {
		 preX = positionX;
		 preY = positionY;

		 int startX = positionX / step - showDist;
		 int startY = positionY / step - showDist;
		 static int objNum = 0;

		 if (startX < 0)
			 startX = 0;
		 if (startY < 0)
			 startY = 0;
		 int maxI = startX + showDist * 2 >= n? n - 1 : startX + showDist * 2;
		 int maxJ = startY + showDist * 2 >= n ? n - 1 : startY + showDist * 2;

		 for (int i = startX; i < maxI; i++) {
			 for (int j = startY; j < maxJ; j++) {
			/*	 if (objNum == 1000)
				 	return;*/

				 if (envObjData[i][j] != 0)
				 {
					 if (envObjData[i][j] == 1) {
						 Object* treeObj = new Object(&treeModel, 1);
						 treeObj->obb.extents.z = treeObj->obb.extents.x;
						 float x = ground->totalV[i][j].position.x;
						 float y = ground->totalV[i][j].position.y;
						 float z = ground->totalV[i][j].position.z;
						 treeObj->setModelMat(glm::vec3(x, y, z - 0.05), glm::vec3(0.001f));
						 treeObj->setModelMat(glm::vec3(0.0f), glm::vec3(1.0f), 90.0*PI / 180, glm::vec3(1.0f, 0.0f, 0.0f));
						 treeObj->setMaterial(glm::vec3(0.5f), glm::vec3(0.5f), 5);
						 r->addObject(treeObj);

						 obj.push_back(treeObj);
						 objPosition.push_back(vec2(x, y));
						 envObjData[i][j] = 0;
					 }
					 else {
						 //Object* grassObj = new Object(&grassModel, 1);
						 //grassObj->obb.extents.z = grassObj->obb.extents.x;
						 //float x = ground->totalV[i][j].position.x;
						 //float y = ground->totalV[i][j].position.y;
						 //float z = ground->totalV[i][j].position.z;
						 //grassObj->setModelMat(glm::vec3(x, y, z - 0.05), glm::vec3(0.001f));
						 //grassObj->setModelMat(glm::vec3(0.0f), glm::vec3(1.0f), 90.0*PI / 180, glm::vec3(1.0f, 0.0f, 0.0f));
						 //grassObj->setMaterial(glm::vec3(0.3f), glm::vec3(0.5f), 5);
						 //r->addObject(grassObj);

						 //obj.push_back(grassObj);
						 //objPosition.push_back(vec2(x, y));
						 envObjData[i][j] = 0;
					 }

					//BaseModel *bm = new BaseModel();
					//bm->loadModel("Cube 1 1 20 ");
					//Object* obm = new Object(bm);
					//obm->setModelMat(glm::vec3(ground->totalV[i][j].position.x, ground->totalV[i][j].position.y, ground->totalV[i][j].position.z), glm::vec3(1.0f), 0, glm::vec3(0.0f, 0.0f, 1.0f));
					//obm->setMaterial(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.35f, 0.35f, 0.35f), 32);
					//r->addObject(obm);
					//envObjData[i][j] = 0;
					objNum++;
					
				 }
			 }
		 }
		 //cout << "显示环境物体个数" << objNum << endl;
		 for (int i = 0; i < obj.size(); i++)
		 {
			 if (abs(objPosition[i].x - positionX) > showDist || abs(objPosition[i].y - positionY) > showDist)
				 obj[i]->toHide();
			 else
				 obj[i]->toShow();
		 }
	 }
	 catch (const std::exception&)
	 {

	 }

 }
