#pragma once

#include "../stdh.h"
#include "../shader.h"
#include "../camera.h"
#include "../texture.h"
#include "../object.h"
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>    

/*
	shader 为 自己根据GLSL所编写的着色器
	uniform为 shader中需要用到的不变量
	VBO绑定shader中需要用到的变量
	VAO绑定一系列的 VBO绑定 语句， 即
		glBindVertexArray(vao_id);
		==
		{
			glBindBuffer(GL_ARRAY_BUFFER, this->vertexVBO);
			glEnableVertexAttribArray(VERTEX_LOC); //Vertex_Loc为shader中该变量的位置
			glVertexAttribPointer(VERTEX_LOC, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);   
			... 
		}
*/
class Object;

enum AttribType { VERTEX_LOC, NORMAL_LOC, TEXCOORD_LOC, BONE_ID, BONE_WE };



class Model
{
public:
	virtual void setBaseUniform(const Shader& shader, const Camera& camera, const Object& object);
	virtual void setDepthUniform(const Shader& depthShader, const glm::mat4& depthVP, Object& object);
	virtual bool loadModel(const std::string& filePath) = 0;//model必须的模型文件   或    纹理文件  的加载
	virtual void draw() const = 0;
	virtual void drawDepthMap() const = 0;
	/*
		shader.useProgram();
		...

		glBindVertexArray(vao_id);
		...
		glDrawElements(...); 根据索引画点   或    glDrawArrays(); 根据坐标画点
		glBindVertexArray(0);

		...
		glUseProgram(0);
	*/
	//virtual void setUniform();//若本model样式根据某时间变化 则需根据time设置uniform
	glm::vec3 pos_info, len_info;
protected:
	std::string modelFileDir; //文件路径
};

