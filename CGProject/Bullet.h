#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include "Shader.h"

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "Model.h"

class Bullet
{
public:
	static float velocity;
	static std::string genRandomString();
	std::string id;
	Bullet(glm::vec3 front = glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f));
	~Bullet();
	void render(Model &bulletModel);
private:
	glm::vec3 position;
	glm::vec3 front;
	Shader *shader;
	void InitialData();
	GLuint VAO, VBO;
};
