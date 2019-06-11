#pragma once
#include "Shader.h"
#include <vector>
#include "Camera.h"
#include "Controler.h"

class SkyBox
{
public:
	SkyBox(std::string skyName);
	~SkyBox();
	void render(Camera camera);

private:
	//Initialize float array and shader
	void InitialData();
	//load cube for skybox
	GLuint loadCubemapTexture(std::vector<std::string> vecSkyfaces);

	GLuint skyboxVAO, skyboxVBO;
	GLuint cubeVAO, cubeVBO;
	GLfloat *skyboxVertices;
	Shader *skyboxShader;
	GLuint textureCubemap;
	std::string sky;
};

