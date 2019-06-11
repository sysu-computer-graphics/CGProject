#pragma once
#include "Controler.h"
#include "Model.h"

class ModelLoadingExample
{
public:
	ModelLoadingExample(const std::string &path);
	~ModelLoadingExample();

	void render();

private:
	Shader *shader;
	Model *model;
};

