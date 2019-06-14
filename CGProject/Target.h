#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"

class Target
{
public:
	Target();
	Target(glm::vec3 _position);
	~Target();
	void render(glm::mat4 projection, glm::mat4 view);
	glm::vec3 getPosition() { return position; }

private:
	glm::vec3 position;
	float angle;
	void InitialData();
	void renderScene(Shader* renderShader);
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

