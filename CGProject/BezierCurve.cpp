#include "BezierCurve.h"
BezierCurve* BezierCurve::instance = nullptr;

BezierCurve::BezierCurve()
{
	this->isDrawing = false;
	this->mouse_x = 0.0;
	this->mouse_y = 0.0;
	this->cur_t = 0.0;
}

BezierCurve::~BezierCurve()
{
}

BezierCurve * BezierCurve::getInstance()
{
	if (instance == nullptr) {
		instance = new BezierCurve();
	}
	return instance;
}

void BezierCurve::render()
{
	Controler::getInstance()->ourShader->use();

	// �������Ƶ�
	for (auto it : this->ctrlPoints) {
		drawPoint(it, 0.03f, this->blueColor);
	}
	// �������Ƶ�֮�������
	for (int i = 1; i < (int)ctrlPoints.size(); i++) {
		drawLine(ctrlPoints[i - 1], ctrlPoints[i]);
	}
	// ����Bezier Curve
	drawBezierCurve();

	// �������� (��enter��)
	if (this->isDrawing) {
		if (this->cur_t <= 1.0) {
			drawProcess(this->ctrlPoints, cur_t);
			cur_t += 0.002;
		}
		else {
			this->isDrawing = false;
			cur_t = 0.0;
		}
	}
}

void BezierCurve::drawPoint(const glm::vec2 &point, const float size, glm::vec3 color)
{
	const float hsize = size / 2;
	float vertices[] = {
		point.x - hsize, point.y - hsize, 0.0f,  color.x, color.y, color.z,
		point.x + hsize, point.y - hsize, 0.0f,  color.x, color.y, color.z,
		point.x + hsize, point.y + hsize, 0.0f,  color.x, color.y, color.z,
		point.x - hsize, point.y + hsize, 0.0f,  color.x, color.y, color.z
	};
	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};
	processObjects(vertices, sizeof(vertices), indices, sizeof(indices));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	freeObjects();
}

void BezierCurve::drawLine(const glm::vec2 &p0, const glm::vec2 &p1, glm::vec3 color)
{
	float vertices[] = {
		p0.x, p0.y, 0.0f, color.x, color.y, color.z,
		p1.x, p1.y, 0.0f, color.x, color.y, color.z
	};
	processObjects(vertices, sizeof(vertices));
	glDrawArrays(GL_LINES, 0, 2);
	freeObjects();
}

double BezierCurve::fact(const int n) const
{
	double fact = 1;
	for (int i = 1; i <= n; i++) {
		fact *= i;
	}
	return fact;
}

void BezierCurve::drawBezierCurve()
{
	if (ctrlPoints.size() < 2)
		return;
	bezierPoints.clear();
	int n = (int)ctrlPoints.size() - 1;
	for (double t = 0; t <= 1.0; t += 0.001) {
		double x = 0, y = 0;
		for (int i = 0; i <= n; i++) {
			double B = fact(n) / fact(i) / fact(n - i) * pow(t, i) * pow(1.0 - t, n - i);
			x += ctrlPoints[i].x * B;
			y += ctrlPoints[i].y * B;
		}
		bezierPoints.push_back(glm::vec2(x, y));
	}

	glm::vec3 color = this->greenColor;
	float *vertices = new float[bezierPoints.size() * 6];
	for (int i = 0; i < (int)bezierPoints.size(); i++) {
		vertices[i * 6 + 0] = (float)bezierPoints[i].x;
		vertices[i * 6 + 1] = (float)bezierPoints[i].y;
		vertices[i * 6 + 2] = 0.0f;
		vertices[i * 6 + 3] = color.x;
		vertices[i * 6 + 4] = color.y;
		vertices[i * 6 + 5] = color.z;
	}

	processObjects(vertices, bezierPoints.size() * 6 * sizeof(vertices));
	glDrawArrays(GL_POINTS, 0, bezierPoints.size());
	delete[]vertices;
	freeObjects();
}

void BezierCurve::drawProcess(const std::vector<glm::vec2>& curCtrlPoints, const double t)
{
	if (curCtrlPoints.size() < 2)
		return;
	std::vector<glm::vec2> nextCtrlPoints;
	for (int i = 0; i < (int)curCtrlPoints.size() - 1; i++) {
		double x = (1.0 - t) * curCtrlPoints[i].x + t * curCtrlPoints[i + 1].x;
		double y = (1.0 - t) * curCtrlPoints[i].y + t * curCtrlPoints[i + 1].y;
		nextCtrlPoints.push_back(glm::vec2(x, y));
	}

	if (nextCtrlPoints.size() == 1) {
		drawPoint(nextCtrlPoints[0], 0.03f, this->redColor);
	}
	else {
		for (int i = 0; i < (int)nextCtrlPoints.size() - 1; i++) {
			drawLine(nextCtrlPoints[i], nextCtrlPoints[i + 1], this->orangeColor);
		}
		drawProcess(nextCtrlPoints, t);
	}
}
	
void BezierCurve::processObjects(float vertices[], GLsizeiptr sizeofVertices, unsigned int indices[], GLsizeiptr sizeofIndices)
{
	// 0. ����VAO VBO EBO����
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// 1. �󶨶����������
	glBindVertexArray(VAO);
	// 2. �����ǵĶ������鸴�Ƶ�һ�����㻺���У���OpenGLʹ��
	// ʹ��glBindBuffer�������´����Ļ���󶨵�GL_ARRAY_BUFFERĿ����
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// ����glBufferData�����������֮ǰ����Ķ������ݸ��Ƶ�������ڴ���
	glBufferData(GL_ARRAY_BUFFER, sizeofVertices, vertices, GL_STATIC_DRAW);
	// 3. �������ǵ��������鵽һ�����������У���OpenGLʹ��
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeofIndices, indices, GL_STATIC_DRAW);
	// 4. �趨����λ������ָ��
	// ʹ��glVertexAttribPointer��������OpenGL����ν�����������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// 5. �趨������ɫ����ָ��
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(VAO);
}

void BezierCurve::freeObjects()
{
	// �ͷ�VAO��VBO��EBO��Դ
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}