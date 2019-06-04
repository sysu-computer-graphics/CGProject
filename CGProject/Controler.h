#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"

/* glm-0.9.9.5 */
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb_image.h>

#include <iostream>
#include <vector>

#include "Shader.h"
#include "Camera.h"
#include "BezierCurve.h"


class Controler
{
public:
	GLFWwindow* window;
	Shader *ourShader;

	// Singleton
	static Controler* getInstance();
	// 初始化GLFW
	bool init(const int scr_width, const int scr_height);
	const unsigned int getScrWidth() const { return this->scr_width; };
	const unsigned int getScrHeight() const { return this->scr_height; };

	/************************* ImGui *************************/
	// 初始化ImGui
	static void initImGui(GLFWwindow *t_window);
	// Start the Dear ImGui frame
	static void startImGui();
	// 渲染循环里调用，渲染ImGui
	static void renderImGui();
	// 释放ImGui资源
	static void freeImGui();

	/**************************************************************/
	// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
	static void processInput(GLFWwindow *window);
	// glfw: whenever the window size changed (by OS or user resize) this callback function executes
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	// glfw: whenever the mouse moves, this callback is called
	static void mouse_callback(GLFWwindow *window, double xpos, double ypos);
	// glfw: whenever the mouse scroll wheel scrolls, this callback is called
	static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
	// mouse button
	static void mouse_button_callback(GLFWwindow * window, int button, int action, int mods);
	// key
	static void key_callback(GLFWwindow *window, int key, int scanmode, int action, int mods);

private:
	Controler();
	~Controler();
	// Singleton: disallow the copy constructor and assign operator
	Controler(const Controler &other) = delete;
	Controler & operator=(const Controler &other) = delete;

	// 窗口大小
	unsigned int scr_width;
	unsigned int scr_height;

	static Controler* instance;

};

