#if !defined(STB_IMAGE_IMPLEMENTATION)
#define STB_IMAGE_IMPLEMENTATION
#endif

#include <stdlib.h>

#include "SkyBox.h"
#include "CGModel.h"
#include "Player.h"
#include "Bullet.h"
#include "Target.h"
#include "FontRenderer.h"
#include "PhysicsEngine.h"
#include "SceneController.h"

float radians = 0.0f;
//target position
glm::vec3 targetPos = glm::vec3(9.0f, 1.8f, 4.5f);

glm::vec3 Controler::lightPos = glm::vec3(100.0f, 50.0f, 0.0f);

int barrierHint = 0;

double limitFPS = 1.0 / 60.0;
int remainTime = 60;

void updateRemainTime() {
	if (remainTime > 0) {
		remainTime--;
	}
	else {
		Controler::isGameOver = true;
	}
}


int main()
{
	/******************************** Initializations ************************************/
	Controler::getInstance()->init(1000, 600);
	glEnable(GL_DEPTH_TEST);	// configure global opengl state, enable depth test
	bool show_demo_window = false;

	// display cursor
	glfwSetInputMode(Controler::getInstance()->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// initial font renderer
	FontRenderer::getInstance()->init();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// PhysicsEngine
	PhysicsEngine::getInstance();

	// skybox
	SkyBox skybox("envmap_miramar");

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 staticViewMat = Controler::getInstance()->camera.getViewMatrix();


	//scene
	//CGModel scene("resources/model/hercules_at_rest/scene.gltf", "GLSL/model_loading.vs", "GLSL/model_loading.fs");
	//model = glm::mat4(1.0f);
	//model = glm::translate(model, glm::vec3(24.0f, 78.0f, 0.0f));
	//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	//scene.setModelMatrix(model);

	Model bulletModel("resources/model/bullet/scene.gltf");
	Player *player = Player::getInstance();
	//Target *target = new Target(targetPos);
	SceneController::getInstance()->init();

	int score = 0;
	double lastTime = glfwGetTime(), timer = lastTime;
	double deltaTime = 0, nowTime = 0;
	int frames = 0, updates = 0;

	/******************************** Render Loop ****************************************/
	while (!glfwWindowShouldClose(Controler::getInstance()->window)) {
		/* 确保摄像机在所有硬件上移动速度都一样 https://learnopengl-cn.github.io/01%20Getting%20started/09%20Camera/#_4 */
		float currentFrame = (float)glfwGetTime();

		deltaTime += (nowTime - lastTime) / limitFPS;
		lastTime = nowTime;

		Controler::deltaTime = currentFrame - Controler::lastFrame;
		Controler::lastFrame = currentFrame;

		Controler::processInput(Controler::getInstance()->window);

		/* ImGui窗口内容 */
		Controler::startImGui();
		{
			// Demo Window
			if (show_demo_window) ImGui::ShowDemoWindow();
			ImGui::Text("(%.3f, %.3f, %.3f)", Controler::camera.getPosition().x, Controler::camera.getPosition().y, Controler::camera.getPosition().z);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			//ImGui::InputFloat("rotate: ", &radians, 1.0f);
		}

		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!
		//Controler::getInstance()->targetManager->render();
		
		while (frames >= 30.0) {
			updateRemainTime();   // - Update function
			updates++;
			deltaTime--;
			frames = 0;
		}

		/*************************** ImGui render **********************************/
		Controler::renderImGui();

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(Controler::camera.getZoom()), (float)Controler::getInstance()->getScrWidth() / (float)Controler::getInstance()->getScrHeight(), 0.1f, 100.0f);
		glm::mat4 view = Controler::camera.getViewMatrix();

		if (PhysicsEngine::hasCollision(SceneController::getInstance())) {
			// 检测到与子弹有碰撞时，分数加一，target的位置随机改变
			score++;
			SceneController::getInstance()->setTargetPosition(rand() % 14);
		}
		if (PhysicsEngine::hasCollisionBarrier(SceneController::getInstance())) {
			// 检测到与障碍物有碰撞三次时，分数减一
			barrierHint++;
			if (barrierHint == 3) {
				if (score > 0) {
					score--;
				}
				barrierHint = 0;
			}
		}
		SceneController::getInstance()->render(projection, view);

		glViewport(0, 0, Controler::getInstance()->getScrWidth(), Controler::getInstance()->getScrHeight());

		/*************************** skybox render **********************************/
		glDepthFunc(GL_LEQUAL);
		skybox.render(Controler::getInstance()->camera);


		/*************************** model render **********************************/
		//scene.render(projection, view);
		if (!Controler::camera.isLock) {
			player->render(staticViewMat);
		}
		// 传入bulletModel模型，绘制子弹
		Controler::getInstance()->bulletManager->render(bulletModel, Controler::lightPos);

		/*************************** ImGui render **********************************/
		Controler::renderImGui();

		// text render example
		FontRenderer::getInstance()->RenderText(std::string("Total Hits: ") + std::to_string(score), 10.0f, 10.0f, 0.5f, glm::vec3(1.0, 0.0f, 0.0f));
		FontRenderer::getInstance()->RenderText(std::string("Remain Time: ") + std::to_string(remainTime), (float)Controler::getInstance()->getScrWidth() / 2 - 100, (float)Controler::getInstance()->getScrHeight() - 40, 0.5f, glm::vec3(1.0, 0.0f, 0.0f));
		//FontRenderer::getInstance()->RenderText("Remain Targets: 20+", 750.0f, 570.0f, 0.5f, glm::vec3(0.5, 0.8f, 0.2f));
		if (Controler::isGameOver) {
			FontRenderer::getInstance()->RenderText(std::string("Game Over! "), (float)Controler::getInstance()->getScrWidth() / 2 - 200, (float)Controler::getInstance()->getScrHeight() / 2, 1.5f, glm::vec3(1.0, 0.0f, 0.0f));
		}

		frames++;

		// - Reset after one second
		if (glfwGetTime() - timer > 1.0) {
			timer++;
			//std::cout << "FPS: " << frames << " Updates:" << updates << std::endl;
			updates = 0, frames = 0;
		}
			
		glfwSwapBuffers(Controler::getInstance()->window);
		glfwPollEvents();
		
	}

	return 0;
}
