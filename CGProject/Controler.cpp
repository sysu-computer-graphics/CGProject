#include "Controler.h"

#include "Player.h"

// ��̬����
Controler* Controler::instance = nullptr;

Camera Controler::camera;
float Controler::deltaTime = 0.0f;
float Controler::lastFrame = 0.0f;
float Controler::lastX = 0.0f;
float Controler::lastY = 0.0f;
bool Controler::firstMouse = true;

Controler::Controler()
{
}

Controler::~Controler()
{
	// ����Ⱦѭ��������������Ҫ��ȷ�ͷ�/ɾ��֮ǰ�ķ����������Դ
	glfwDestroyWindow(window);
	glfwTerminate();
	window = nullptr;

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
 * ��ʼ��GLFW, glad, ImGui, ����shader, �Լ�ע����ֻص�����
 * @param scr_width ���ڿ���
 * @param scr_height ���ڸ߶�
 * @return if success, true will be returned
 */
bool Controler::init(const int scr_width, const int scr_height)
{
	this->scr_width = scr_width;
	this->scr_height = scr_height;
	Controler::lastX = scr_width / 2.0f;
	Controler::lastY = scr_height / 2.0f;
	Controler::firstMouse = true;

	glfwInit();	// ��ʼ��GLFW

	// ����GLFW����Ҫʹ�õ�OpenGL�汾��3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);	// ���汾��(Major)��Ϊ3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);	// �ΰ汾��(Minor)��Ϊ3

	// ��ȷ����GLFW����ʹ�õ��Ǻ���ģʽ(Core-profile)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// ���ڵĿ��͸���Ϊ����ǰ��������, ������������ʾ������ڵ����ƣ����⣩
	this->window = glfwCreateWindow(scr_width, scr_height, "LearnOpenGL", NULL, NULL);
	if (this->window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	// ֪ͨGLFW�����Ǵ��ڵ�����������Ϊ��ǰ�̵߳���������
	glfwMakeContextCurrent(this->window);
	glfwSwapInterval(1); // Enable vsync

	// �ڵ����κ�OpenGL�ĺ���֮ǰ������Ҫ��ʼ��GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	// ע����ֻص�����
	glfwSetFramebufferSizeCallback(this->window, framebuffer_size_callback);
	glfwSetCursorPosCallback(this->window, mouse_callback);
	glfwSetScrollCallback(this->window, scroll_callback);
	glfwSetMouseButtonCallback(this->window, mouse_button_callback);
	glfwSetKeyCallback(this->window, key_callback);

	// ��ʼ��ImGui
	Controler::initImGui(Controler::getInstance()->window);

	return true;
}

void Controler::resetCamera()
{
	Controler::lastX = 0.0f;
	Controler::lastY = 0.0f;
	Controler::firstMouse = true;
	Controler::camera.reset();
}

/**
 * ��ʼ��ImGui
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
	// ��esc�����رմ���
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);

	Player * player = Player::getInstance();
	// ���� W S A D ��������ƶ�
	// Controler::camera.processKeyBoard(Camera::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) player->onKeyDown(Camera::FORWARD, deltaTime);
	// Controler::camera.processKeyBoard(Camera::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) player->onKeyDown(Camera::BACKWARD, deltaTime);
	// Controler::camera.processKeyBoard(Camera::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) player->onKeyDown(Camera::LEFT, deltaTime);
	// Controler::camera.processKeyBoard(Camera::RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) player->onKeyDown(Camera::RIGHT, deltaTime);

	// ����ctrl�����������
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) Controler::resetCamera();
}

void Controler::framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void Controler::mouse_callback(GLFWwindow * window, double xpos, double ypos)
{
	// ����������ƶ�
	if (Controler::firstMouse) {
		Controler::lastX = (float)xpos;
		Controler::lastY = (float)ypos;
		Controler::firstMouse = false;
	}
	float xoffset = (float)xpos - lastX;
	float yoffset = lastY - (float)ypos; // reversed since y-coordinates go from bottom to top
	Controler::lastX = (float)xpos;
	Controler::lastY = (float)ypos;
	// update player's Perspective (expressed as player's body rotate & camera's dir)
	Player::getInstance()->onMouseMove(xoffset, yoffset);
}

void Controler::scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
	// �����ָı��������Ұ(field of view, fov)
	camera.processMouseScroll((float)yoffset);
}

void Controler::mouse_button_callback(GLFWwindow * window, int button, int action, int mods)
{
	// BezierCurve::getInstance()->mouse_button_callback_draw(button, action);
}

void Controler::key_callback(GLFWwindow * window, int key, int scanmode, int action, int mods)
{
	// BezierCurve::getInstance()->key_callback_show_process(key, action);
}
