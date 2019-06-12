#if !defined(STB_IMAGE_IMPLEMENTATION)
#define STB_IMAGE_IMPLEMENTATION
#endif

#include "SkyBox.h"
#include "ModelLoadingExample.h"
#include "CGModel.h"

int main()
{
	/******************************** Initializations ************************************/
	Controler::getInstance()->init(1200, 800);
	glEnable(GL_DEPTH_TEST);	// configure global opengl state, enable depth test
	bool show_demo_window = false;

	// display cursor
	glfwSetInputMode(Controler::getInstance()->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// skybox
	SkyBox skybox("envmap_miramar");

	// AK12, a rifle with hands
	CGModel ak12("resources/model/AK12/sf2arms.obj", "GLSL/model_loading.vs", "GLSL/model_loading.fs");
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.2f, -5.5f, 6.2f));
	model = glm::rotate(model, glm::radians(190.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
	ak12.setModelMatrix(model);
	glm::mat4 ak12ViewMatrix = Controler::getInstance()->camera.getViewMatrix();

	// nanosuit
	CGModel nanosuit("resources/model/nanosuit/nanosuit.obj", "GLSL/model_loading.vs", "GLSL/model_loading.fs");
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
	nanosuit.setModelMatrix(model);

	/******************************** Render Loop ****************************************/
	while (!glfwWindowShouldClose(Controler::getInstance()->window)) {
		/* 确保摄像机在所有硬件上移动速度都一样 https://learnopengl-cn.github.io/01%20Getting%20started/09%20Camera/#_4 */
		float currentFrame = (float)glfwGetTime();
		Controler::deltaTime = currentFrame - Controler::lastFrame;
		Controler::lastFrame = currentFrame;

		Controler::processInput(Controler::getInstance()->window);

		/* ImGui窗口内容 */
		Controler::startImGui();
		{
			// Demo Window
			if (show_demo_window) ImGui::ShowDemoWindow();
			ImGui::Checkbox("Demo Window", &show_demo_window);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}

		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(Controler::camera.getZoom()), (float)Controler::getInstance()->getScrWidth() / (float)Controler::getInstance()->getScrHeight(), 0.1f, 100.0f);
		glm::mat4 view = Controler::camera.getViewMatrix();

		/*************************** skybox render **********************************/
		glDepthFunc(GL_LEQUAL);
		skybox.render(Controler::getInstance()->camera);

		/*************************** model render **********************************/
		nanosuit.render(projection, view);
		ak12.render(projection, ak12ViewMatrix);

		/*************************** ImGui render **********************************/
		Controler::renderImGui();

		glfwSwapBuffers(Controler::getInstance()->window);
		glfwPollEvents();
	}

	return 0;
}