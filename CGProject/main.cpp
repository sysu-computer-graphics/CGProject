#if !defined(STB_IMAGE_IMPLEMENTATION)
#define STB_IMAGE_IMPLEMENTATION
#endif

#include "SkyBox.h"
#include "Player.h"
#include "Bullet.h"

float rotate[3] = { 0.0f, 1.0f, 0.0f };
float radians = 0.0f;

int main()
{
	/******************************** Initializations ************************************/
	Controler::getInstance()->init(800, 800);
	glEnable(GL_DEPTH_TEST);	// configure global opengl state, enable depth test
	bool show_demo_window = false;

	glfwSetInputMode(Controler::getInstance()->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // display cursor
	// skybox
	SkyBox skybox("envmap_miramar");
	// An example of model loading and rendering
	Player player("resources/model/nanosuit/nanosuit.obj");
	// bullet example
	Bullet bullet;

	/******************************** Render Loop ****************************************/
	while (!glfwWindowShouldClose(Controler::getInstance()->window)) {
		/* ȷ�������������Ӳ�����ƶ��ٶȶ�һ�� https://learnopengl-cn.github.io/01%20Getting%20started/09%20Camera/#_4 */
		float currentFrame = (float)glfwGetTime();
		Controler::deltaTime = currentFrame - Controler::lastFrame;
		Controler::lastFrame = currentFrame;

		Controler::processInput(Controler::getInstance()->window);

		/* ImGui�������� */
		Controler::startImGui();
		{
			// Demo Window
			if (show_demo_window) ImGui::ShowDemoWindow();
			ImGui::Checkbox("Demo Window", &show_demo_window);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::InputFloat("rotate: ", &radians, 1.0f);
		}

		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

		/*************************** skybox render **********************************/
		glDepthFunc(GL_LEQUAL);
		skybox.render(Controler::getInstance()->camera);

		/*************************** model render **********************************/
		player.setRotate(radians);
		player.render();
		bullet.render();

		/*************************** ImGui render **********************************/
		Controler::renderImGui();

		glfwSwapBuffers(Controler::getInstance()->window);
		glfwPollEvents();
	}

	return 0;
}