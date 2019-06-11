#pragma once
#include "Shader.h"
#include <vector>
#include "Camera.h"
#include "Controler.h"

using namespace std;

class SkyBox
{
public:
	SkyBox(string skyName);
	~SkyBox();
	void render(Camera camera);

private:
	//Initialize float array and shader
	void InitialData();
	//load cube for skybox
	GLuint loadCubemapTexture(std::vector<string> vecSkyfaces);

	GLuint skyboxVAO, skyboxVBO;
	GLuint cubeVAO, cubeVBO;
	GLfloat *skyboxVertices;
	Shader *skyboxShader;
	GLuint textureCubemap;
	string sky;
};

