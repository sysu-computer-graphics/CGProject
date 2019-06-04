#include "Controler.h"

// 静态变量
Controler* Controler::instance = nullptr;

Controler::Controler()
{
}

Controler::~Controler()
{
	// 当渲染循环结束后我们需要正确释放/删除之前的分配的所有资源
	glfwDestroyWindow(window);
	glfwTerminate();
	window = nullptr;

	if (ourShader) {
		delete ourShader;
		ourShader = nullptr;
	}
	if (instance) {
		delete instance;
		instance = nullptr;
	}

	Controler::freeImGui();
}

Controler * Controler::getInstance()
{
	if (instance == nullptr) {
		instance = new Controler();
	}
	return instance;
}

/**
 * 初始化GLFW, glad, ImGui, 编译shader, 以及注册各种回调函数
 * @param scr_width 窗口宽度
 * @param scr_height 窗口高度
 * @return if success, true will be returned
 */
bool Controler::init(const int scr_width, const int scr_height)
{
	this->scr_width = scr_width;
	this->scr_height = scr_height;
	/*Controler::lastX = scr_width / 2.0f;
	Controler::lastY = scr_height / 2.0f;*/

	glfwInit();	// 初始化GLFW

	// 告诉GLFW我们要使用的OpenGL版本是3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);	// 主版本号(Major)设为3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);	// 次版本号(Minor)设为3

	// 明确告诉GLFW我们使用的是核心模式(Core-profile)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// 窗口的宽和高作为它的前两个参数, 第三个参数表示这个窗口的名称（标题）
	this->window = glfwCreateWindow(scr_width, scr_height, "LearnOpenGL", NULL, NULL);
	if (this->window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	// 通知GLFW将我们窗口的上下文设置为当前线程的主上下文
	glfwMakeContextCurrent(this->window);
	glfwSwapInterval(1); // Enable vsync

	// 在调用任何OpenGL的函数之前我们需要初始化GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	// 注册各种回调函数
	glfwSetFramebufferSizeCallback(this->window, framebuffer_size_callback);
	glfwSetCursorPosCallback(this->window, mouse_callback);
	glfwSetScrollCallback(this->window, scroll_callback);
	glfwSetMouseButtonCallback(this->window, mouse_button_callback);
	glfwSetKeyCallback(this->window, key_callback);

	// 编译shader
	this->ourShader = new Shader("GLSL/shader.vs", "GLSL/shader.fs");

	// 初始化ImGui
	Controler::initImGui(Controler::getInstance()->window);

	return true;
}

/**
 * 初始化ImGui
 * @param t_window GLFWwindow
 * @return void
 */
void Controler::initImGui(GLFWwindow * t_window)
{
	const char* glsl_version = "#version 130";
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(t_window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
}

void Controler::startImGui()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void Controler::renderImGui()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Controler::freeImGui()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void Controler::processInput(GLFWwindow * window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
}

void Controler::framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void Controler::mouse_callback(GLFWwindow * window, double xpos, double ypos)
{
	BezierCurve::getInstance()->mouse_x = xpos;
	BezierCurve::getInstance()->mouse_y = ypos;
}

void Controler::scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
}

void Controler::mouse_button_callback(GLFWwindow * window, int button, int action, int mods)
{
	if (action == GLFW_PRESS && !BezierCurve::getInstance()->isDrawing) {
		switch (button)
		{
		case GLFW_MOUSE_BUTTON_LEFT: {
			float x = 2.0f * (float)BezierCurve::getInstance()->mouse_x / (float)Controler::getInstance()->getScrWidth() - 1.0f;
			float y = 1.0f - 2.0f * (float)BezierCurve::getInstance()->mouse_y / (float)Controler::getInstance()->getScrHeight();
			BezierCurve::getInstance()->ctrlPoints.push_back(glm::vec2(x, y));
		} break;
		case GLFW_MOUSE_BUTTON_RIGHT: {
			if (!BezierCurve::getInstance()->ctrlPoints.empty()) {
				BezierCurve::getInstance()->ctrlPoints.pop_back();
			}
		} break;
		default: break;
		}
	}
}

void Controler::key_callback(GLFWwindow * window, int key, int scanmode, int action, int mods)
{
	if (action == GLFW_PRESS && key == GLFW_KEY_ENTER) {
		if (BezierCurve::getInstance()->isDrawing) {
			BezierCurve::getInstance()->cur_t = 0.0;
		}
		BezierCurve::getInstance()->isDrawing = !BezierCurve::getInstance()->isDrawing;
	}
}
