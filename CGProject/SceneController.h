#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"

class SceneController
{
public:
	~SceneController();
	void render(glm::mat4 projection, glm::mat4 view);
	glm::vec3 getTargetPosition() { return targetPosition; }
	void setTargetPosition(const glm::vec3 &pos) { this->targetPosition = pos; }
	static SceneController* getInstance();
	void init();
private:
	static SceneController* instance;
	SceneController();
	void renderScene(Shader* renderShader);
	glm::vec3 targetPosition;
	float angle;
	GLuint cubeVAO, cubeVBO;
	GLuint planeVAO, planeVBO;
	GLuint myTexture;

	GLuint depthMapFBO, depthMap;
	const GLuint SHADOW_WINDTH = 1024;
	const GLuint SHADOW_HEIGHT = 1024;
	Shader* shader;
	Shader* simpleDepthShader;
	Shader* debugDepthQuad;
	float ambientStrength, specularStrength, diffuseStrength, shininess;
};
