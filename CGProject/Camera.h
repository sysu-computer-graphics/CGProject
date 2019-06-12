#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <iostream>

class Camera
{
public:
	enum CameraMovement { FORWARD, BACKWARD, LEFT, RIGHT };
	
	// Constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 3.3f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
	// Constructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
	~Camera();
	void reset();
	float getZoom() const { return zoom; };
	glm::vec3 getPosition() const { return position; }

	// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 getViewMatrix() const;
	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void processKeyBoard(const CameraMovement direction, const float deltaTime);
	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void processMouseScroll(float yoffset);
	glm::vec3 getFrontVec();
private:
	// Default camera values
	static const float YAW;
	static const float PITCH;
	static const float SPEED;
	static const float SENSITIVITY;
	static const float ZOOM;

	// Camera Attributes
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;
	// Euler Angles
	float yaw;
	float pitch;
	// Camera options
	float movementSpeed;
	float mouseSensitivity;
	float zoom;

	// used for reset to the init state
	glm::vec3 init_position;
	glm::vec3 init_up;
	float init_yaw;
	float init_pitch;

	// Calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors();
};

