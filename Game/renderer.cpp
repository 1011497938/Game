#include "renderer.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>

timeb Renderer::start;

float Renderer::getRunningTime()
{
	timeb t;
	ftime(&t);
	int dif = (t.time * 1000 + t.millitm) - (start.time * 1000 + start.millitm);

	return dif / 1000.0;
}

Renderer::Renderer(const Camera& camera, const Shader& shader) :camera(camera), shader(shader)
{
	shader.useProgram();
	glUniform1i(glGetUniformLocation(shader.getID(), "Texture_D"), Texture::TEXTURE_DIFFUSE);
	glUniform1i(glGetUniformLocation(shader.getID(), "Texture_S"), Texture::TEXTURE_SPECULAR);
	glUniform1i(glGetUniformLocation(shader.getID(), "ShadowMap"), Texture::TEXTURE_SHADOW);
}

Renderer::~Renderer()
{

}

void Renderer::renderDepthMap(const glm::mat4& depthVP, const Shader& depthShader, const int& flag)
{
	depthShader.useProgram();
	for (auto t = objects.begin(); t !=objects.end(); t++)
	{
		if ((*t)->toDelete) {
			delete (*t);
			objects.erase(t);
		}
		if ((*t)->getState())//&&t!=objects.at(objects.size()-1))
		{
			(*t)->drawDepthMap(depthShader, depthVP, flag);
		}
	}
	glUseProgram(0);
}

void Renderer::addObject(Object* m)
{
	objects.push_back(m);
}

void Renderer::render()
{
	shader.useProgram();
	glUniform3fv(glGetUniformLocation(shader.getID(), "CameraPosition"), 1, glm::value_ptr(camera.getPos()));
	glUniform3fv(glGetUniformLocation(shader.getID(), "CenterPos"), 1, glm::value_ptr(center));

	for (Object* t : objects)
	{
		if (t->getState())
		{
			t->draw(shader, camera);
		}
	}
	glUseProgram(0);
}

/*
#include <ft2build.h>
#include FT_FREETYPE_H
#pragma comment(lib, "freetype28d.lib")
TextRenderer::TextRenderer(const Camera& camera, const Shader& fontShader) :BaseRenderer(camera), fontShader(fontShader)
{
	setDrawArrays(GL_QUADS, 0, 4);
	glUniform1i(glGetUniformLocation(fontShader.getID(), "TextureID"), Texture::TEXTURE_NORMAL);
	vboID = addVBO(BaseRenderer::VERTEX_LOC, NULL, 12);
	GLfloat data[] =
	{
		0.0, 1.0,
		0.0, 0.0,
		1.0, 0.0,
		1.0, 1.0
	};
	addVBO(BaseRenderer::TEXCOORD_LOC, data, 8);
	addEND();
}

TextRenderer::~TextRenderer()
{
	for (auto i = Characters.begin(); i != Characters.end(); i++)
		delete i->second;
}

void TextRenderer::load(const std::string& font, const GLuint& fontSize)
{
	for (auto i = Characters.begin(); i != Characters.end(); i++)
		delete i->second;
	this->Characters.clear();

	FT_Library ft;
	FT_Face face;
	if (FT_Init_FreeType(&ft))
		throw Error("Could not init FreeType Library");
	if (FT_New_Face(ft, font.c_str(), 0, &face))
		throw Error("Failed to load font");
	FT_Set_Pixel_Sizes(face, 0, fontSize);

	FT_UInt charIndex;
	charIndex = FT_Get_Char_Index(face, 'a');
	FT_Load_Glyph(face, charIndex, FT_LOAD_DEFAULT);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (GLubyte c = 0; c < 128; c++) // lol see what I did there
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}

		Character *character = new Character{
			Texture(GL_CLAMP_TO_EDGE, GL_RED, GL_RED),
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			(GLuint)face->glyph->advance.x
		};
		character->TextureID.generate(face->glyph->bitmap.width, face->glyph->bitmap.rows, face->glyph->bitmap.buffer);
		Characters.insert(std::pair<GLchar, Character*>(c, character));
	}
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

void TextRenderer::renderText(const std::string& text, const glm::vec3& pos, const bool& onScreen, const glm::vec3& color, const GLfloat& scaleSize)
{	
	glUniform3fv(glGetUniformLocation(fontShader.getID(), "Color"), 1, glm::value_ptr(color));
	glm::mat4 MVP;
	if (onScreen)
		MVP = glm::mat4(1.0f);
	else
	{
		glm::mat4 model;
		model = glm::translate(model, pos);
		//model = glm::scale(model, glm::vec3(0.1f));// scaleSize);
		glm::mat4 modelView = camera.getViewMatrix() * model;
		MVP = camera.getProjectionMatrix() * modelView;
	}
	glUniformMatrix4fv(glGetUniformLocation(fontShader.getID(), "MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
	GLfloat scale = scaleSize*0.01;
	GLfloat t = 0;
	for (auto c = text.begin(); c != text.end(); c++)
	{
		Character* ch = Characters[*c];
		GLfloat xpos = t + ch->Bearing.x * scale;
		GLfloat ypos = (ch->Bearing.y - ch->Size.y) * scale ;
		GLfloat w = ch->Size.x * scale;
		GLfloat h = ch->Size.y * scale;
		GLfloat vertices[] = {
			 xpos,	ypos,	0.0 ,
			 xpos,	ypos+h,	0.0 ,
			 xpos+w,ypos+h,	0.0 ,
			 xpos+w,ypos,	0.0 
		};
		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

		ch->TextureID.bind(Texture::TEXTURE_NORMAL);
		glBindVertexArray(vaoID);
		glDrawArrays(mode, first, count);
		glBindVertexArray(0);

		t += (ch->Advance >> 6) * scale;
	}
}*/