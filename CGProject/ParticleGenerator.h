#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include "Shader.h"

#include "Controler.h"

struct Particle {
	glm::vec2 Position, Velocity;
	glm::vec4 Color;
	float xrange;
	GLfloat Life;

	Particle()
		: Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f), xrange(0.0f) { }
};

class ParticleGenerator
{
public:
	ParticleGenerator(GLuint number);
	~ParticleGenerator();
	void updateParticles(float dt, GLuint num, glm::vec2 position, glm::vec2 velocity, glm::vec2 offset);
	void renderParticles();

private:
	GLuint nr_particles = 0;
	std::vector<Particle> particles;
	GLuint lastUsedParticle = 0;
	Shader* particleShader = nullptr;
	GLuint particleTexture;
	GLuint VAO;
	void init();
	void RespawnParticle(Particle &particle, glm::vec2 position, glm::vec2 velocity, glm::vec2 offset);
	GLuint FirstUnusedParticle();
	GLfloat middleX;
};


