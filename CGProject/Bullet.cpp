#include "Bullet.h"
#include "Controler.h"
#include "Player.h"

float Bullet::velocity = 1.0f;

Bullet::Bullet(glm::vec3 front, glm::vec3 pos)
{
	this->position = pos;
	this->front = front;
	this->shader = new Shader("GLSL/bullet.vs", "GLSL/bullet.fs");
	this->id = Bullet::genRandomString();
	InitialData();
}


Bullet::~Bullet()
{
	if (shader) {
		delete shader;
		shader = nullptr;
	}
}

void Bullet::InitialData()
{
	GLfloat vertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

}

void Bullet::render() {
	// don't forget to enable shader before setting uniforms
	this->shader->use();

	// view/projection transformations
	glm::mat4 projection = glm::perspective(glm::radians(Controler::camera.getZoom()), (float)Controler::getInstance()->getScrWidth() / (float)Controler::getInstance()->getScrHeight(), 0.1f, 100.0f);
	glm::mat4 view = Controler::camera.getViewMatrix();
	this->shader->setMat4("projection", projection);
	this->shader->setMat4("view", view);

	// render the loaded model
	// 根据子弹的方向 front 和 position 计算出每一帧运动后的位置

	glm::mat4 model = glm::mat4(1.0f);
	this->position = this->position + this->front * Bullet::velocity;
	model = glm::translate(model, this->position);


	// 如果子弹离玩家角色距离很远很远 自动回收
	float distance = glm::distance(this->position, Player::getInstance()->getPosition());
	if (distance > 1000)
		Controler::getInstance()->bulletManager->deleteBullet(this->id);


	//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 8.0f)); // translate it down so it's at the center of the scene
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.2f));	// it's a bit too big for our scene, so scale it down
	this->shader->setMat4("model", model);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

std::string Bullet::genRandomString() {
	srand(time(NULL));
	std::string res = "";
	for (int i = 0; i < 20; i++) {
		switch ((rand() % 3))
		{
		case 1:
			res += 'A' + rand() % 26;
			break;
		case 2:
			res = 'a' + rand() % 26;
			break;
		case 3:
			res = '0' + rand() % 10;
			break;
		default:
			break;
		}
	}
	return res;
}
