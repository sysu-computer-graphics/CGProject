#pragma once
#include "Model.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class CGModel :
	public Model
{
public:
	CGModel(const std::string &path, const GLchar * vertexPath, const GLchar * fragmentPath);
	~CGModel();

	// ����һ�� CGModel �����Ҫͨ�� setModelMatrix �������ú�ģ�͵� model ���� (�Լ�����)
	void setModelMatrix(const glm::mat4 &model);

	// ����Ⱦѭ���е���
	void render(const glm::mat4 &projection, const glm::mat4 &view);

private:
	Shader *shader;
	glm::mat4 model;
};

