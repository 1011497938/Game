#pragma once

#include "stdh.h"
#include "camera.h"
#include "object.h"
#include "Ground\ground.h"

class Callback
{
public:
	Callback(GLFWwindow* window);
	~Callback();
	static void setCamera(Camera* c) { camera = c; }
	static void setObject(Object* o) { object = o; }
	static void setTerrain(Object* o) { terrain = o; }
	static void setGround(Ground* g) { ground = g; }
	static void setHightmap(Image* i) { image = i; }
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void scroll_callback(GLFWwindow* window, double dx, double dy);
	static void cursor_pos_callback(GLFWwindow* window, double dx, double dy);

	static void moving();

	static bool isWalking, isAttack;
	static vector<Object*> boundList;
	static glm::vec2 forward, upvec;
	static float dx;

private:
	static void flashViewMat();
	static void rotateX(const float& angle);
	static void rotateY(const float& angle);
	static bool isObjInTerrain(double dx, double dy);
	static double calDeltaZ(double dx, double dy);

	GLFWwindow* window;
	static Ground* ground;
	static Camera* camera;
	static Object* object, *terrain;
	static bool MOUSE_LEFT, MOUSE_MIDDLE, MOUSE_RIGHT, KEY_ALT;
	static Image* image;
	static float back, back_x, up, center_h, dy;
	static int selected;
};
