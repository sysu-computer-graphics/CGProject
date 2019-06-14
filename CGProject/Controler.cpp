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
	if (bulletManager) {
		delete bulletManager;
		bulletManager = nullptr;
	}

	Controler::freeImGui();
}

GLuint Controler::loadTexture(std::string path)
{
	//����
	unsigned int texture = 0;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	//���Ʒ�ʽ�������ظ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	//������ˣ���Сʱʹ������ڣ��Ŵ�ʱʹ�����Է�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//����ͼƬ
	int pic_width, pic_height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path.c_str(), &pic_width, &pic_height, &nrChannels, 0);
	//��������
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, pic_width, pic_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	return texture;
}

void Controler::loadDepthMap()
{
	glGenFramebuffers(1, &(this->depthMapFBO));
	const GLuint SHADOW_WINDTH = 1024, SHADOW_HEIGHT = 1024;

	glGenTextures(1, &(this->depthMap));
	glBindTexture(GL_TEXTURE_2D, this->depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	//���Ʒ�ʽ�������ظ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	//������ˣ���Сʱʹ������ڣ��Ŵ�ʱʹ�����Է�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, this->depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
 * @param scr_width ���ڿ��
 * @param scr_height ���ڸ߶�
 * @return if success, true will be returned
 */
bool Controler::init(const int scr_width, const int scr_height)
{
	this->scr_width = scr_width;
	this->scr_height = scr_height;
	this->bulletManager = new BulletManager();

	Controler::lastX = scr_width / 2.0f;
	Controler::lastY = scr_height / 2.0f;
	Controler::firstMouse = true;

	glfwInit();	// ��ʼ��GLFW

	// ����GLFW����Ҫʹ�õ�OpenGL�汾��3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);	// ���汾��(Major)��Ϊ3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);	// �ΰ汾��(Minor)��Ϊ3

	// ��ȷ����GLFW����ʹ�õ��Ǻ���ģʽ(Core-profile)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// ���ڵĿ�͸���Ϊ����ǰ��������, ������������ʾ������ڵ����ƣ����⣩
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
	
	this->simpleDepthShader = new Shader("GLSL/SimpleDepthShader.vs", "GLSL/SimpleDepthShader.fs");
	this->shadowMappingShader = new Shader("GLSL/shadow_mapping.vs", "GLSL/shadow_mapping.fs");
	this->debugDepthQuadShader = new Shader("GLSL/quad_depth.vs", "GLSL/quad_depth.fs");

	loadDepthMap();
	
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

	if (!Controler::camera.isLock) {
		Player * player = Player::getInstance();
		// ���� W S A D ������ҽ�ɫ�ƶ�
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) player->onKeyDown(Camera::FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) player->onKeyDown(Camera::BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) player->onKeyDown(Camera::LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) player->onKeyDown(Camera::RIGHT, deltaTime);
	}

	// ����ctrl���������ϵ��ӽ�
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && !Controler::camera.isLock) {
		if (!Controler::camera.isLock) {
			Controler::camera.isLock = true;
			Player::getInstance()->startCloseUp();
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE && Controler::camera.isLock) {
		Player::getInstance()->endCloseUp();
		Controler::camera.isLock = false;
	}
}

void Controler::framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void Controler::mouse_callback(GLFWwindow * window, double xpos, double ypos)
{
	if (Controler::camera.isLock) return;
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
	if (action == GLFW_PRESS) {
		switch (button)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
			glm::vec3 front = Controler::camera.getFrontVec();
			glm::vec3 pos = Controler::camera.getPosition();
			Controler::getInstance()->bulletManager->newBullet(front, pos + front * 2.0f);
			break;
		default:
			break;
		}
	}
}

void Controler::key_callback(GLFWwindow * window, int key, int scanmode, int action, int mods)
{
	// BezierCurve::getInstance()->key_callback_show_process(key, action);
}
