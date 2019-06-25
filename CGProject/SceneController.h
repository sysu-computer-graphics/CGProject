#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Shader.h"
#include "CGModel.h"

class SceneController
{
public:
	~SceneController();
	void render(glm::mat4 projection, glm::mat4 view);
	glm::vec3 getTargetPosition() { return targetPosition; }
	void setTargetPosition(int pos) { this->targetPosition = this->targetPositionSet[pos]; }
	static SceneController* getInstance();
	std::vector<glm::vec3> getBarrierPositions() { return barrierPositionSet; }
	void init();
private:
	static SceneController* instance;
	SceneController();
	void renderScene(Shader* renderShader);
	void initData();
	glm::vec3 targetPosition;
	float angle;
	GLuint cubeVAO, cubeVBO;
	GLuint planeVAO, planeVBO;
	GLuint groundTexture, targetTexture, barrierTexture;

	CGModel* Muwu;

	bool firstRender = true;
	GLuint depthMapFBO, depthMap;
	const GLuint SHADOW_WINDTH = 1024;
	const GLuint SHADOW_HEIGHT = 1024;
	Shader* shader;
	Shader* simpleDepthShader;
	Shader* debugDepthQuad;
	float ambientStrength, specularStrength, diffuseStrength, shininess;
	std::vector<glm::vec3> targetPositionSet;
	std::vector<glm::vec3> barrierPositionSet;
	std::vector<glm::mat4> barrierModelList;
};
