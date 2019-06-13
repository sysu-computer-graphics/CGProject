#pragma once
#include "Controler.h"
#include "Model.h"
#include <glm/gtx/vector_angle.hpp>

class Player
{
public:
	// Player(const std::string const &path);
	// ~Player();
	void render();
	void setPosition(glm::vec3 pos);
	void setRotate(float radians);
	void onMouseMove(float xoffset, float yoffset);
	void onKeyDown(const Camera::CameraMovement direction, const float deltaTime);
	void startCloseUp();
	void endCloseUp();
	static Player * getInstance()
	{
		if (instance == nullptr) {
			instance = new Player();
		}
		return instance;
	}
private:
	Player(const std::string const &path = "resources/model/nanosuit/nanosuit.obj");
	~Player();
	static Player* instance;
	Shader *shader;
	Model *model;
	glm::vec3 position;
	float radians;
	float movementSpeed = 5.0f;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	void updatePlayerVectors();
};

