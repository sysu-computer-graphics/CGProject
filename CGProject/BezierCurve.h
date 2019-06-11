#pragma once
#include "Controler.h"
#include <math.h>

/* Homework 8 - Bezier Curve */
class BezierCurve
{
public:
	// Singleton
	static BezierCurve* getInstance();

	void render();
	void setMouse(double xpos, double ypos);
	void mouse_button_callback_draw(int button, int action);
	void key_callback_show_process(int key, int action);
	
	bool isDrawing;
	double mouse_x;
	double mouse_y;
	double cur_t;
	std::vector<glm::vec2> ctrlPoints;

private:
	BezierCurve();
	~BezierCurve();
	// Singleton: disallow the copy constructor and assign operator
	BezierCurve(const BezierCurve &other) = delete;
	BezierCurve & operator=(const BezierCurve &other) = delete;

	const glm::vec3 blueColor = glm::vec3(0.2588f, 0.4588f, 0.9255f);
	const glm::vec3 greenColor = glm::vec3(0.0549f, 0.8627f, 0.5333f);
	const glm::vec3 redColor = glm::vec3(0.9686f, 0.2157f, 0.3843f);
	const glm::vec3 orangeColor = glm::vec3(0.9765f, 0.6431f, 0.2510f);

	static BezierCurve* instance;
	Shader *ourShader;
	std::vector<glm::vec2> bezierPoints;
	
	void drawPoint(const glm::vec2 &point, const float size = 0.03f, glm::vec3 color = glm::vec3(0.5f, 0.53f, 0.56f));
	void drawLine(const glm::vec2 &p0, const glm::vec2 &p1, glm::vec3 color = glm::vec3(0.5f, 0.53f, 0.56f));
	double fact(const int n) const;
	void drawBezierCurve();
	void drawProcess(const std::vector<glm::vec2> &curCtrlPoints, const double t);

	unsigned int VBO, VAO, EBO;
	void processObjects(float vertices[], GLsizeiptr sizeofVertices, unsigned int indices[] = nullptr, GLsizeiptr sizeofIndices = 0);
	void freeObjects();
};

