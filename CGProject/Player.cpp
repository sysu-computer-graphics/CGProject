#include "Player.h"

Player* Player::instance = nullptr;

Player::Player(const std::string & path)
{
	this->shader = new Shader("GLSL/model_loading.vs", "GLSL/model_loading.fs");
	this->model = new Model(path);
	this->position = glm::vec3(0.0f, 0.0f, 0.0f);
	this->radians = 0.0f;
	this->up = glm::vec3(0.0f, 1.0f, 0.0f);
	this->front = glm::vec3(0.0f, 0.0f, 1.0f);
	this->right = glm::vec3(-1.0f, 0.0f, 0.0f);
}


Player::~Player()
{
	if (shader) {
		delete shader;
		shader = nullptr;
	}
	if (model) {
		delete model;
		model = nullptr;
	}
	if (instance) {
		delete instance;
		instance = nullptr;
	}
}

void Player::setPosition(glm::vec3 pos) {
	this->position = pos;
}

void Player::setRotate(float radians) {
	this->radians = radians;
}


void Player::render(glm::mat4 view)
{
	// don't forget to enable shader before setting uniforms
	this->shader->use();

	// view/projection transformations
	glm::mat4 projection = glm::perspective(glm::radians(Controler::camera.getZoom()), (float)Controler::getInstance()->getScrWidth() / (float)Controler::getInstance()->getScrHeight(), 0.1f, 100.0f);
	this->shader->setMat4("projection", projection);
	this->shader->setMat4("view", view);

	// render the loaded model
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-0.15f, -2.2f, 1.6f));
	model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-4.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));

	this->shader->setMat4("model", model);
	this->model->draw(*this->shader);
}

void Player::onMouseMove(float xoffset, float yoffset) {
	// ���������
	Controler::camera.processMouseMovement(xoffset, yoffset);
	// update front, right vector
	updatePlayerVectors();
}

void Player::onKeyDown(const Camera::CameraMovement direction, const float deltaTime) {
	float velocity = movementSpeed * deltaTime;
	if (direction == Camera::CameraMovement::FORWARD) position += front * velocity;
	if (direction == Camera::CameraMovement::BACKWARD) position -= front * velocity;
	if (direction == Camera::CameraMovement::LEFT) position -= right * velocity;
	if (direction == Camera::CameraMovement::RIGHT) position += right * velocity;
	// �����ͬ�������ƶ�
	Controler::camera.processKeyBoard(direction, deltaTime);
}

void Player::updatePlayerVectors() {
	// �ӽǱ仯ʱ������ҵĳ���
	glm::vec3 cameraFrontVec = Controler::camera.getFrontVec();
	glm::vec3 oldFront = glm::vec3(0.0f, 0.0f, 1.0f);
	this->front = glm::normalize(glm::vec3(cameraFrontVec.x, 0.0f, cameraFrontVec.z));
	this->right = glm::normalize(glm::cross(this->front, up));
	// xoffset, yoffset -> rotate radians
	glm::vec3 nf = glm::normalize(this->front);
	glm::vec3 no = glm::normalize(oldFront);

	// �������ģ�͵ĽǶ�
	float radians;
	if (nf.x < 0) {
		radians = 6.28f - glm::acos(glm::dot(nf, no));
	}
	else {
		radians = glm::acos(glm::dot(nf, no));
	}
	this->setRotate(radians);
}

void Player::startCloseUp() {
	// �����ϵ��ӽ�
	// glm::vec3 dir = glm::vec3(front.x, 1.0f, front.z);
	glm::vec3 dir = glm::vec3(0.0f, 1.0f, 0.0f);
	Controler::camera.startCloseUp(dir * 10.0f);
}

void Player::endCloseUp() {
	Controler::camera.endCloseUp();
}
