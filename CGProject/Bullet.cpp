#include "Bullet.h"
#include "Controler.h"
#include "Player.h"

float Bullet::velocity = 1.0f;

Bullet::Bullet(glm::vec3 front, glm::vec3 pos)
{
	this->position = pos;
	this->front = front;
	this->shader = new Shader("GLSL/bullet_light.vs", "GLSL/bullet_light.fs");
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

void Bullet::render(Model &bulletModel, const glm::vec3 &_lightPos) {
	// don't forget to enable shader before setting uniforms
	this->shader->use();

	// view/projection transformations
	glm::mat4 projection = glm::perspective(glm::radians(Controler::camera.getZoom()), (float)Controler::getInstance()->getScrWidth() / (float)Controler::getInstance()->getScrHeight(), 0.1f, 100.0f);
	glm::mat4 view = Controler::camera.getViewMatrix();
	this->shader->setMat4("projection", projection);
	this->shader->setMat4("view", view);

	// render the loaded model
	// �����ӵ��ķ��� front �� position �����ÿһ֡�˶����λ��

	glm::mat4 model = glm::mat4(1.0f);
	this->position = this->position + this->front * Bullet::velocity;
	model = glm::translate(model, this->position);


	// ����ӵ�����ҽ�ɫ�����Զ��Զ �Զ�����
	float distance = glm::distance(this->position, Player::getInstance()->getPosition());
	if (distance > 10000)
		Controler::getInstance()->bulletManager->deleteBullet(this->id);


	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	// �ӵ�����ǰyaw, pitch��ת��ʹ������ӵ�ʱֻ��������β��
	model = glm::rotate(model, glm::radians(-Controler::camera.getYaw()), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(Controler::camera.getPitch()), glm::vec3(0.0f, 0.0f, 1.0f));
	this->shader->setMat4("model", model);

	// ��������
	float ambientStrength = 0.5f;
	float diffuseStrength = 1.0f;
	float specularStrength = 0.8f;
	int shininess = 64;
	this->shader->setFloat("ambientStrength", ambientStrength);
	this->shader->setFloat("diffuseStrength", diffuseStrength);
	this->shader->setFloat("specularStrength", specularStrength);
	this->shader->setInt("shininess", shininess);
	this->shader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	this->shader->setVec3("lightPos", _lightPos);
	this->shader->setVec3("viewPos", Player::getInstance()->getPosition());

	bulletModel.draw(*this->shader);
	
	/*glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);*/
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
