#include "CGModel.h"



CGModel::CGModel(const std::string &path, const GLchar * vertexPath, const GLchar * fragmentPath) : Model(path)
{
	this->shader = new Shader(vertexPath, fragmentPath);
	this->model = glm::mat4(1.0f);
}

CGModel::~CGModel()
{
	if (shader) {
		delete shader;
		shader = nullptr;
	}
}

void CGModel::setModelMatrix(const glm::mat4 & model)
{
	this->model = model;
}

void CGModel::render(const glm::mat4 &projection, const glm::mat4 &view)
{
	// don't forget to enable shader before setting uniforms
	this->shader->use();

	this->shader->setMat4("projection", projection);
	this->shader->setMat4("view", view);
	this->shader->setMat4("model", this->model);

	// render the loaded model
	Model::draw(*this->shader);
}

