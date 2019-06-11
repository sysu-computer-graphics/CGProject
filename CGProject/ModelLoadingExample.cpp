#include "ModelLoadingExample.h"

ModelLoadingExample::ModelLoadingExample(const std::string const & path)
{
	this->shader = new Shader("GLSL/model_loading.vs", "GLSL/model_loading.fs");
	this->model = new Model(path);
}

ModelLoadingExample::~ModelLoadingExample()
{
	if (shader) {
		delete shader;
		shader = nullptr;
	}
	if (model) {
		delete model;
		model = nullptr;
	}
}

void ModelLoadingExample::render()
{
	// don't forget to enable shader before setting uniforms
	this->shader->use();

	// view/projection transformations
	glm::mat4 projection = glm::perspective(glm::radians(Controler::camera.getZoom()), (float)Controler::getInstance()->getScrWidth() / (float)Controler::getInstance()->getScrHeight(), 0.1f, 100.0f);
	glm::mat4 view = Controler::camera.getViewMatrix();
	this->shader->setMat4("projection", projection);
	this->shader->setMat4("view", view);

	// render the loaded model
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
	this->shader->setMat4("model", model);
	this->model->draw(*this->shader);
}

