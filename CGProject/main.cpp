#if !defined(STB_IMAGE_IMPLEMENTATION)
#define STB_IMAGE_IMPLEMENTATION
#endif

#include "BezierCurve.h"

int main()
{
	/******************************** Initializations ************************************/
	Controler::getInstance()->init(800, 800);
	Controler::initImGui(Controler::getInstance()->window);

	BezierCurve::getInstance();
	glEnable(GL_DEPTH_TEST);	// configure global opengl state, enable depth test
	bool show_demo_window = false;

	/******************************** Render Loop ****************************************/
	while (!glfwWindowShouldClose(Controler::getInstance()->window)) {
		Controler::processInput(Controler::getInstance()->window);

		Controler::startImGui();
		// Demo Window
		if (show_demo_window) ImGui::ShowDemoWindow();
		ImGui::Checkbox("Demo Window", &show_demo_window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

		/*************************** render **********************************/
		BezierCurve::getInstance()->render();
		Controler::renderImGui();

		glfwSwapBuffers(Controler::getInstance()->window);
		glfwPollEvents();
	}

	Controler::freeImGui();
	return 0;
}