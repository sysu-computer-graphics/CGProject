#pragma once
#include "Controler.h"
#include "Model.h"

class ModelLoadingExample
{
public:
	ModelLoadingExample(const std::string const &path);
	~ModelLoadingExample();

	void render();

private:
	Shader *shader;
	Model *model;
};

