#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <map>
#include <iostream>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Shader.h"

struct Character {
	GLuint     TextureID;  // ���������ID
	glm::ivec2 Size;       // ���δ�С
	glm::ivec2 Bearing;    // �ӻ�׼�ߵ�������/������ƫ��ֵ
	GLuint     Advance;    // ԭ�����һ������ԭ��ľ���
};


class FontRenderer
{
public:
	std::map<GLchar, Character> Characters;
	glm::mat4 projection = glm::ortho(0.0f, 1000.0f, 0.0f, 600.0f);

	GLuint VAO;
	GLuint VBO;
	void init();
	void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
	Shader * shader;

	static FontRenderer * getInstance()
	{
		if (instance == nullptr) {
			instance = new FontRenderer();
		}
		return instance;
	}


private:
	static FontRenderer* instance;
	void initCharacter();
	void initData();
	FontRenderer();
	~FontRenderer();
};
