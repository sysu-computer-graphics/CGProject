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

	// 创建一个 CGModel 对象后，要通过 setModelMatrix 方法设置好模型的 model 矩阵 (自己调参)
	void setModelMatrix(const glm::mat4 &model);

	// 在渲染循环中调用
	void render(const glm::mat4 &projection, const glm::mat4 &view);

private:
	Shader *shader;
	glm::mat4 model;
};

