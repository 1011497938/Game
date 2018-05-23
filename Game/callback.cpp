#include "callback.h"

Ground* Callback::ground = NULL;
Camera* Callback::camera = NULL;
Object* Callback::object = NULL;
Object* Callback::terrain = NULL;
Image* Callback::image = NULL;
float Callback::back = -9, Callback::back_x = -9, Callback::up = 0, Callback::center_h = 4.8;
glm::vec2 Callback::forward = glm::vec2(1.0f, 0.0f), Callback::upvec = glm::vec2(0.0f, 1.0f);
bool Callback::MOUSE_LEFT = false, Callback::MOUSE_MIDDLE = false, Callback::MOUSE_RIGHT = false, Callback::isWalking = false, Callback::isAttack = false, Callback::KEY_ALT = false;
int Callback::selected = 0;
float Callback::dx, Callback::dy;
vector<Object*> Callback::boundList;

Callback::Callback(GLFWwindow* window) :window(window)
{
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);
}

Callback::~Callback()
{
	glfwSetKeyCallback(window, NULL);
	glfwSetMouseButtonCallback(window, NULL);
	glfwSetScrollCallback(window, NULL);
	glfwSetCursorPosCallback(window, NULL);
}

void Callback::flashViewMat()
{
	glm::vec3 t = glm::vec3(object->obb.center.x, object->obb.center.y, center_h);
	camera->setViewMatrix(t + glm::vec3(back_x * forward, 1.3f + up), 
		t + glm::vec3(0.0f, 0.0f, 1.3f),
		glm::vec3(upvec.x*forward, -upvec.y));
}

bool Callback::isObjInTerrain(double dx, double  dy)
{
	if (image == NULL)
		return false;
	if (object->obb.center.x + dx < 0 || object->obb.center.x + dx> image->height*image->size)
		return false;
	if (object->obb.center.y + dy < 0 || object->obb.center.y + dy > image->width*image->size)
		return false;
	return true;
}

double Callback::calDeltaZ(double dx, double  dy)
{
	double xf, yf, z;
	int x0, y0;
	float step = ground->step;

	x0 = (int)((object->obb.center.x) / step);
	y0 = (int)((object->obb.center.y) / step);
	xf = 1 - (((object->obb.center.x) / step) - x0);
	yf = 1 - (((object->obb.center.y) / step) - y0);
	z = (ground->getHeight(x0, y0) * xf + ground->getHeight(x0 + 1, y0) * (1 - xf)) * yf
		+ (ground->getHeight(x0, y0 + 1) * xf + ground->getHeight(x0 + 1, y0 + 1) * (1 - xf)) * (1 - yf);
	center_h = z;

	return z - object->obb.center_d.z;
}

void Callback::rotateX(const float& angle)
{
	GLfloat cs = cos(angle);
	GLfloat sn = sin(angle);
	glm::vec2 t(forward);
	forward.x = cs*t.x + sn*t.y;
	forward.y = cs*t.y - sn*t.x;
}

void Callback::rotateY(const float& angle)
{
	static float ang = 0;
	if (ang + angle > PI / 2)
		ang = PI / 2;
	else if (ang + angle < - PI / 2)
		ang = - PI / 2;
	else
		ang += angle;
	GLfloat cs = cos(ang);
	GLfloat sn = sin(ang);
	back_x = back*cs;
	up = back*sn;
	upvec.y = cs;
	upvec.x = sn;
}

void Callback::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	static float dz = 0;
	if (action == GLFW_PRESS)
		if (mods == GLFW_MOD_ALT) {
			KEY_ALT = true;
		}
		else {
			KEY_ALT = false;
		}
		switch (key)
		{
		case GLFW_KEY_W:
			dx = 0.2;
			break;
		case GLFW_KEY_S:
			dx = -0.2;
			break;
		case GLFW_KEY_A:
			dy = 0.2;
			break;
		case GLFW_KEY_D:
			dy = -0.2;
			break;
		case GLFW_KEY_SPACE:
			dz = 0.2;
			break;
		case GLFW_KEY_C:
			dz = -0.2;
			break;
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		default:
			break;
		}
	if (action == GLFW_RELEASE)
		switch (key)
		{
		case GLFW_KEY_W:
		case GLFW_KEY_S:
			dx = 0;
			break;
		case GLFW_KEY_A:
		case GLFW_KEY_D:
			dy = 0;
			break;
		case GLFW_KEY_C:
		case GLFW_KEY_SPACE:
			dz = 0;
			break;
		case GLFW_KEY_TAB:
			selected = 1 - selected;
			if (selected == 1) {
				//camera->setViewMatrix(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
			}
			break;
		default:
			break;
		}
}

void Callback::moving() {
	if (selected == 1) {
		camera->move(dx, dy, 0);
	}
	else if(!isAttack){
		if (dx)
			isWalking = true;
		else
			isWalking = false;
		double z = 0;
		double x_n = dx * 20;
		object->setModelMat(glm::vec3(x_n, 0, 0));
		if (!object->obb.isIntersect(terrain->obb))
			object->setModelMat(glm::vec3(-x_n, 0, 0));
		else {
			int flag = 0;
			for (Object* o : boundList) {
				if (object->obb.isIntersect(o->obb))
				{
					flag = 1;
					object->setModelMat(glm::vec3(-x_n, 0, 0));
					break;
				}
			}
			if (flag == 0)
				z = calDeltaZ(x_n*forward.x, x_n*forward.y);
		}
		float angle = (dy * 10)*PI / 180;
		object->setModelMat(glm::vec3(0, z/0.03, 0), glm::vec3(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
		rotateX(-angle);
		flashViewMat();
	}
}

void Callback::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS) switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		MOUSE_LEFT = true;
		if (selected == 0)
			isAttack = true;
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		MOUSE_MIDDLE = true;
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		MOUSE_RIGHT = true;
		break;
	}
	else if(action == GLFW_RELEASE) switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		MOUSE_LEFT = false;
		if (selected == 0)
			isAttack = false;
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		MOUSE_MIDDLE = false;
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		MOUSE_RIGHT = false;
		break;
	}
	return;
}

void Callback::scroll_callback(GLFWwindow* window, double dx, double dy)
{
	if (selected == 1)
	{
		camera->changeDist((dx - dy)*10);
		back -= (dx - dy)*10;
	}
	else
	{
		if (back - (dx - dy) < -10)
		{
			back = -10;
		}
		else if (back - (dx - dy) > -2)
		{
			back = -2;
		}
		else {
			back -= dx - dy;
		}
		rotateY(0);
		flashViewMat();
	}
}

void Callback::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	static double oldXpos = xpos, oldYpos = ypos;
	double dx = xpos - oldXpos;
	double dy = ypos - oldYpos;
	oldXpos = xpos;
	oldYpos = ypos;

	if (selected == 1) {
		if (MOUSE_LEFT)
			camera->move(dx / 50, -dy / 50, 0);
		else if (MOUSE_RIGHT)
		{
			camera->rotateX(-dx / 20);
			camera->rotateY(dy / 20);
		}
	}
	else if(selected == 0)
	{
		float angle = (dx / 20)*PI / 180;
		if(!KEY_ALT)
			object->setModelMat(glm::vec3(0.0f), glm::vec3(1.0f), -angle, glm::vec3(0.0f, 1.0f, 0.0f));	
		rotateX(angle);
		angle = (dy / 20)*PI / 180;
		rotateY(angle);
		flashViewMat();
	}

}