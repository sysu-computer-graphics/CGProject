#pragma once
#include "Controler.h"
#include "Model.h"

class Player
{
public:
	Player(const std::string const &path);
	~Player();
	void render();
	void setPosition(glm::vec3 pos);
	void setRotate(float radians);
private:
	Shader *shader;
	Model *model;
	glm::vec3 position;
	float radians;
};

