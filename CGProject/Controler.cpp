#include "Controler.h"

#include "Player.h"

// 静态变量
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
	// 当渲染循环结束后我们需要正确释放/删除之前的分配的所有资源
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
	//纹理
	unsigned int texture = 0;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	//环绕方式：镜像重复
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	//纹理过滤：缩小时使用最近邻，放大时使用线性方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//加载图片
	int pic_width, pic_height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path.c_str(), &pic_width, &pic_height, &nrChannels, 0);
	//生成纹理
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
	//环绕方式：镜像重复
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	//纹理过滤：缩小时使用最近邻，放大时使用线性方式
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
 * 初始化GLFW, glad, ImGui, 编译shader, 以及注册各种回调函数
 * @param scr_width 窗口宽度
 * @param scr_height 窗口高度
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

	// 初始化ImGui
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
	// 按esc键，关闭窗口
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);

	if (!Controler::camera.isLock) {
		Player * player = Player::getInstance();
		// 键盘 W S A D 控制玩家角色移动
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) player->onKeyDown(Camera::FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) player->onKeyDown(Camera::BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) player->onKeyDown(Camera::LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) player->onKeyDown(Camera::RIGHT, deltaTime);
	}

	// 按左ctrl键，进入上帝视角
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
	// 鼠标控制相机移动
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
	// 鼠标滚轮改变相机的视野(field of view, fov)
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
