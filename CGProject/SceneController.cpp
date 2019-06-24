#include "SceneController.h"
#include "Shader.h"
#include "Controler.h"
#include "CGModel.h"

SceneController* SceneController::instance = nullptr;

SceneController::SceneController() { }

SceneController::~SceneController() {
	if (instance) {
		delete instance;
	}
	delete Muwu;
}

SceneController* SceneController::getInstance() {
	if (instance == nullptr) {
		instance = new SceneController();
	}
	return instance;
}

void SceneController::initData() {
	// 目标位置 14个
	targetPositionSet.push_back(glm::vec3(30.0f, 4.0f, -50.0f));
	targetPositionSet.push_back(glm::vec3(80.0f, 4.0f, -50.0f));
	targetPositionSet.push_back(glm::vec3(45.0f, 4.0f, -80.0f));
	targetPositionSet.push_back(glm::vec3(60.0f, 4.0f, -80.0f));
	targetPositionSet.push_back(glm::vec3(-50.0f, 4.0f, -50.0f));
	targetPositionSet.push_back(glm::vec3(30.0f, 4.0f, -50.0f));
	targetPositionSet.push_back(glm::vec3(-30.0f, 4.0f, 0.0f));
	targetPositionSet.push_back(glm::vec3(30.0f, 4.0f, 0.0f));
	targetPositionSet.push_back(glm::vec3(-30.0f, 4.0f, 50.0f));
	targetPositionSet.push_back(glm::vec3(-80.0f, 4.0f, 50.0f));
	targetPositionSet.push_back(glm::vec3(30.0f, 4.0f, -20.0f));
	targetPositionSet.push_back(glm::vec3(7.0f, 4.0f, 60.0f));
	targetPositionSet.push_back(glm::vec3(40.0f, 4.0f, -40.0f));
	targetPositionSet.push_back(glm::vec3(-10.0f, 4.0f, -50.0f));

	// 障碍
	for (int i = 0; i < targetPositionSet.size(); i++) {
		barrierPositionSet.push_back(targetPositionSet[i] + glm::vec3(-8.0f, 0.0f, 0.0f));
		barrierPositionSet.push_back(targetPositionSet[i] + glm::vec3(8.0f, 0.0f, 0.0f));
		barrierPositionSet.push_back(targetPositionSet[i] + glm::vec3(0.0f, 0.0f, -8.0f));
		barrierPositionSet.push_back(targetPositionSet[i] + glm::vec3(0.0f, 0.0f, 8.0f));
		barrierPositionSet.push_back(targetPositionSet[i] + glm::vec3(6.0f, 0.0f, 8.0f));
		barrierPositionSet.push_back(targetPositionSet[i] + glm::vec3(-5.0f, 0.0f, -5.0f));
		barrierPositionSet.push_back(targetPositionSet[i] + glm::vec3(8.0f, 0.0f, -5.0f));
		barrierPositionSet.push_back(targetPositionSet[i] + glm::vec3(-5.0f, 0.0f, 5.0f));

		for (int j = 0; j < 8; j++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, barrierPositionSet[i*8 + j] );
			model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f));
			model = glm::rotate(model, glm::radians((float)(rand() % 90)), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::rotate(model, glm::radians((float)(rand() % 90)), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians((float)(rand() % 90)), glm::vec3(1.0f, 0.0f, 0.0f));
			float temp = (float)(rand() % 150) / 100.0f + 0.3;
			model = glm::scale(model, glm::vec3(temp, temp, temp));
			barrierModelList.push_back(model);
		}
	}
}

void SceneController::init() {

	//glm::mat4 model = glm::mat4(1.0f);

	//Muwu = new CGModel("resources/model/hercules_at_rest/scene.gltf", "GLSL/model_loading.vs", "GLSL/model_loading.fs");
	//model = glm::translate(model, glm::vec3(30.0f, 30.0f, 0.0f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	//model = glm::translate(model, glm::vec3(0.0f, -0.25f, 0.0f));
	//model = glm::translate(model, glm::vec3(30.0f, 30.0f, 0.0f));
	
	//Muwu->setModelMatrix(model);

	initData();

	simpleDepthShader = Controler::getInstance()->simpleDepthShader;
	shader = Controler::getInstance()->shadowMappingShader;
	debugDepthQuad = Controler::getInstance()->debugDepthQuadShader;

	GLfloat planeVertices[] = {
		100.0, -0.5f, 100.0, 0.0f, 1.0f, 0.0f, 5.0f, 0.0f,
		-100.0, -0.5f, -100.0, 0.0f, 1.0f, 0.0f, 0.0f, 5.0f,
		-100.0, -0.5f, 100.0, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,

		100.0, -0.5f, 100.0, 0.0f, 1.0f, 0.0f, 5.0f, 0.0f,
		100.0, -0.5f, -100.0, 0.0f, 1.0f, 0.0f, 5.0f, 5.0f,
		-100.0, -0.5f, -100.0, 0.0f, 1.0f, 0.0f, 0.0f, 5.0f
	};

	//plane
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	// position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	// normal attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	// texture attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glBindVertexArray(0);

	groundTexture = Controler::loadTexture("resources/picture/plane.jpg");
	targetTexture = Controler::loadTexture("resources/picture/pic.png");

	depthMap = Controler::getInstance()->depthMap;
	depthMapFBO = Controler::getInstance()->depthMapFBO;

	//顶点数据：坐标和颜色
	GLfloat vertices[] = {
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
		1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
		1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
		// front face
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
		1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
		1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
		1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
		-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
		// left face
		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
		-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
		-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
		// right face
		1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
		1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
		1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
		1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
		1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
		1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
		// bottom face
		-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
		1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
		1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
		1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
		-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
		-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
		// top face
		-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
		1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
		1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
		1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
		-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
		-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left 
	};

	shader->use();
	shader->setInt("material.diffuseTexture", 0);
	shader->setInt("shadowMap", 1);
	debugDepthQuad->use();
	debugDepthQuad->setInt("depthMap", 0);

	//cube
	cubeVBO = 0;
	cubeVAO = 0;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindVertexArray(cubeVAO);
	// position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	ambientStrength = 0.4f;
	specularStrength = 0.4f;
	diffuseStrength = 0.4f;
	shininess = 32.0f;

	this->angle = 0.0f;
	this->targetPosition = targetPositionSet[rand()%14];

}

void SceneController::render(glm::mat4 projection, glm::mat4 view) {
	//Muwu->render(projection, view);
	this->angle += 0.5f;
	//渲染深度图
	GLfloat near_plane = 0.1f, far_plane = 200.0f;
	glm::mat4 lightProjection = glm::mat4(1.0f);
	lightProjection = glm::ortho(-70.0f, 70.0f, -70.0f, 70.0f, near_plane, far_plane);
	glm::mat4 lightView = glm::lookAt(Controler::lightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;
	simpleDepthShader->use();
	simpleDepthShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);

	glViewport(0, 0, SHADOW_WINDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_FRONT);
	
	renderScene(simpleDepthShader);
	
	glCullFace(GL_BACK);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	//渲染背景颜色
	int view_width, view_height;
	glfwGetFramebufferSize(Controler::getInstance()->window, &view_width, &view_height);
	glViewport(0, 0, view_width, view_height);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClear(GL_DEPTH_BUFFER_BIT);
	//Muwu->render(projection, view);
	//渲染真实图像
	shader->use();

	shader->setMat4("projection", projection);
	shader->setMat4("view", view);
	shader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	shader->setVec3("lightPos",Controler::lightPos);
	shader->setVec3("viewPos", Controler::getInstance()->camera.getPosition());
	shader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
	shader->setFloat("ambientStrength", ambientStrength);
	shader->setFloat("specularStrength", specularStrength);
	shader->setFloat("diffuseStrength", diffuseStrength);
	shader->setFloat("material.shininess", shininess);
	//Muwu->render(projection, view);
	renderScene(shader);
	//Muwu->render(projection, view);
	/*
	//显示light的位置
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, Controler::lightPos);
	shader->setMat4("model", model);
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	*/
	debugDepthQuad->use();
	debugDepthQuad->setFloat("near_plane", near_plane);
	debugDepthQuad->setFloat("far_plane", far_plane);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthMap);

	//Muwu->render(projection, view);
}

void SceneController::renderScene(Shader* renderShader) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, groundTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMap);

	glm::mat4 model = glm::mat4(1.0f);

	renderShader->setMat4("model", model);
	
	glBindVertexArray(planeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, targetTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	model = glm::mat4(1.0f);
	model = glm::translate(model, targetPosition);
	model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f));
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(this->angle), glm::vec3(0.0f, 0.0f, 1.0f));
	renderShader->setMat4("model", model);
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	for (int i = 0; i < barrierPositionSet.size(); ++i) {
		renderShader->setMat4("model", barrierModelList[i]);
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}
}