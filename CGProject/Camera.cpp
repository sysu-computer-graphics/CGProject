#include "Camera.h"

// Default camera values
const float Camera::YAW = 90.0f;
const float Camera::PITCH = 0.0f;
const float Camera::SPEED = 5.0f;
const float Camera::SENSITIVITY = 0.1f;
const float Camera::ZOOM = 45.0f;

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
{
	this->front = glm::vec3(0.0f, 0.0f, -1.0f);
	this->movementSpeed = SPEED;
	this->mouseSensitivity = SENSITIVITY;
	this->zoom = ZOOM;

	this->position = position;
	this->worldUp = up;
	this->yaw = yaw;
	this->pitch = pitch;

	this->init_position = position;
	this->init_up = up;
	this->init_yaw = yaw;
	this->init_pitch = pitch;
	updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
{
	this->front = glm::vec3(0.0f, 0.0f, -1.0f);
	this->movementSpeed = SPEED;
	this->mouseSensitivity = SENSITIVITY;
	this->zoom = ZOOM;

	this->position = glm::vec3(posX, posY, posZ);
	this->worldUp = glm::vec3(upX, upY, upZ);
	this->yaw = yaw;
	this->pitch = pitch;

	this->init_position = glm::vec3(posX, posY, posZ);
	this->init_up = glm::vec3(upX, upY, upZ);
	this->init_yaw = yaw;
	this->init_pitch = pitch;
	updateCameraVectors();
}

Camera::~Camera()
{
}

void Camera::reset()
{
	this->front = glm::vec3(0.0f, 0.0f, -1.0f);
	this->movementSpeed = SPEED;
	this->mouseSensitivity = SENSITIVITY;
	this->zoom = ZOOM;

	this->position = this->init_position;
	this->worldUp = this->init_up;
	this->yaw = this->init_yaw;
	this->pitch = this->init_pitch;

	updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(position, position + front, up);
}

void Camera::processKeyBoard(const CameraMovement direction, const float deltaTime)
{
	float velocity = movementSpeed * deltaTime;
	if (direction == FORWARD) position += front * velocity;
	if (direction == BACKWARD) position -= front * velocity;
	if (direction == LEFT) position -= right * velocity;
	if (direction == RIGHT) position += right * velocity;
}

void Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch) {
		if (pitch > 89.0f) pitch = 89.0f;
		if (pitch < -89.0f) pitch = -89.0f;
	}

	// Update Front, Right and Up Vectors using the updated Euler angles
	updateCameraVectors();
}

void Camera::processMouseScroll(float yoffset)
{
	if (zoom > 1.0f && zoom <= 45.0f) zoom -= yoffset;
	if (zoom <= 1.0f) zoom = 1.0f;
	if (zoom >= 45.0f) zoom = 45.0f;
}

void Camera::updateCameraVectors()
{
	// Calculate the new Front vector
	glm::vec3 temp_front;
	temp_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	temp_front.y = sin(glm::radians(pitch));
	temp_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(temp_front);
	// Also re-calculate the Right and Up vector
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}
