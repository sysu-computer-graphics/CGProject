#pragma once
#include "Controler.h"

class Bullet
{
public:
	Bullet();
	~Bullet();
	void render();
private:
	Shader *shader;
	void InitialData();
	GLuint VAO, VBO;
};
