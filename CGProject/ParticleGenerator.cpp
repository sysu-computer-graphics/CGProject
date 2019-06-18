#include "ParticleGenerator.h"
#include "FontRenderer.h"

ParticleGenerator::ParticleGenerator(GLuint number)
{
	this->nr_particles = number;
	lastUsedParticle = 0;
	particleShader = new Shader("GLSL/particle.vs", "GLSL/particle.fs");
	particleTexture = Controler::loadTexture("resources/picture/image1.jpg");
	init();
}

ParticleGenerator::~ParticleGenerator()
{
	particles.clear();
}

void ParticleGenerator::updateParticles(float dt, GLuint num, glm::vec2 position, glm::vec2 velocity, glm::vec2 offset) {
	// Add new particles
	for (GLuint i = 0; i < num; ++i)
	{
		int unusedParticle = FirstUnusedParticle();
		RespawnParticle(particles[unusedParticle], position, velocity, offset);
	}
	// Uupdate all particles
	for (GLuint i = 0; i < nr_particles; ++i)
	{
		Particle &p = particles[i];
		p.Life -= (dt*0.3); // reduce life
		if (p.Life > 0.0f)
		{   // particle is alive, thus update
			p.Position += p.Velocity * dt;
			p.Position.x = p.Life * p.xrange + middleX;
			p.Color.a -= (dt*0.3) * 2.5;
		}
	}
}

GLuint ParticleGenerator::FirstUnusedParticle()
{
	// Search from last used particle, this will usually return almost instantly
	for (GLuint i = lastUsedParticle; i < nr_particles; ++i) {
		if (particles[i].Life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	// Otherwise, do a linear search
	for (GLuint i = 0; i < lastUsedParticle; ++i) {
		if (particles[i].Life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	// Override first particle if all others are alive
	lastUsedParticle = 0;
	return 0;
}


void ParticleGenerator::RespawnParticle(Particle &particle, glm::vec2 position, glm::vec2 velocity, glm::vec2 offset)
{
	this->middleX = position.x;
	GLfloat randomX = ((rand() % 40) - 20) / 10.0f;
	GLfloat randomY = ((rand() % 10) - 5) / 10.0f;
	GLfloat rColor = 0.5 + ((rand() % 100) / 100.0f);
	particle.Position = position + glm::vec2(randomX, randomY);
	particle.xrange = particle.Position.x - position.x;
	particle.Color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	particle.Life = 1.0f;
	particle.Velocity = velocity;
}

void ParticleGenerator::renderParticles() {
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	//Shader *shader = new Shader("GLSL/test.vs", "GLSL/test.fs");
	Shader *shader = particleShader;

	for (Particle particle : particles)
	{
		if (particle.Life > 0.0f)
		{
			shader->use();
			glm::mat4 projection = glm::perspective(Controler::camera.getZoom(), (float)Controler::getInstance()->getScrWidth() / (float)Controler::getInstance()->getScrHeight(), 0.1f, 100.0f);
			shader->setMat4("projection", projection);
			glm::mat4 view = Controler::camera.getViewMatrix();
			shader->setMat4("view", view);
			shader->setVec4("color", particle.Color);
			shader->setVec2("offset", particle.Position);
			std::cout << particle.Position.x << " " << particle.Position.y << std::endl;
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, particleTexture);

			glBindVertexArray(this->VAO);

			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}
	}
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
}

void ParticleGenerator::init()
{
	// Set up mesh and attribute properties
	GLuint VBO;
	GLfloat particle_quad[] = {
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(this->VAO);
	// Fill mesh buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
	// Set mesh attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	for (GLuint i = 0; i < nr_particles; ++i)
		particles.push_back(Particle());
}

