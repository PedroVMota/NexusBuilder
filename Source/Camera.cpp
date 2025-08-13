#include "Camera.h"
#include "main.h"

Camera::Camera(Vec3 position, Vec3 up, float yaw, float pitch)
	: front(Vec3(0.0f, 0.0f, -1.0f)), moveSpeed(SPEED), 
	  mouseSensitivity(SENSITIVITY), zoom(ZOOM), aspectRatio(16.0f/9.0f) {
	this->position = position;
	this->worldUp = up;
	this->yaw = yaw;
	this->pitch = pitch;
	updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ,
			   float yaw, float pitch)
	: front(Vec3(0.0f, 0.0f, -1.0f)), moveSpeed(SPEED),
	  mouseSensitivity(SENSITIVITY), zoom(ZOOM), aspectRatio(16.0f/9.0f) {
	position = Vec3(posX, posY, posZ);
	worldUp = Vec3(upX, upY, upZ);
	this->yaw = yaw;
	this->pitch = pitch;
	updateCameraVectors();
}

Mat4 Camera::GetViewMatrix() const {
	return glm::lookAt(position, position + front, up);
}

Mat4 Camera::GetProjectionMatrix() const {
	return glm::perspective(glm::radians(zoom), aspectRatio, NEAR, FAR);
}

void Camera::ProcessKeyboard(Movement direcction, float deltaTime) {
	float velocity = moveSpeed * deltaTime;

	switch (direcction)
	{
	case Movement::FORWARD:
		position += front * velocity;
		break;
	case Movement::BACKWARD:
		position -= front * velocity;
		break;
	case Movement::RIGHT:
		position += right * velocity;
		break;
	case Movement::LEFT:
		position -= right * velocity;
		break;
	case Movement::UP:
		position += up * velocity;
		break;
	case Movement::DOWN:
		position -= up * velocity;
		break;
	}
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch) {
	xOffset *= mouseSensitivity;
	yOffset *= mouseSensitivity;
	
	yaw += xOffset;
	pitch += yOffset;

	if (constrainPitch) {
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}
	updateCameraVectors();
}

void Camera::ProcessMousePan(float xOffset, float yOffset) {
	float sensitivity = 0.01f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	position -= right * xOffset;
	position += up * yOffset;
}

void Camera::ProcessMouseScroll(float yOffset) {
	zoom -= yOffset;
	if (zoom < 1.0f)
		zoom = 1.0f;
	if (zoom > 45.0f)
		zoom = 45.0f;
}

void Camera::OrbitAround(const Vec3 &target, float xOffset, float yOffset) {
	float distance = glm::length(position - target);

	Vec3 offset = position - target;
	float radius = glm::length(offset);
	float theta = atan2(offset.z, offset.x); // Azimuth angle
	float phi = acos(offset.y / radius); // Polar angle

	theta += xOffset * mouseSensitivity * 0.01f;
	phi -= yOffset * mouseSensitivity * 0.01f;

	phi = std::clamp(phi, 0.1f, 3.14f - 0.1f);

	position.x = target.x + radius * sin(phi) * cos(theta);
	position.y = target.y + radius * cos(phi);
	position.z = target.z + radius * sin(phi) * sin(theta);

	LookAt(target);
}


void Camera::LookAt(const Vec3 &target) {
	Vec3 direction = glm::normalize(target - position);

	yaw = glm::degrees(atan2(direction.z, direction.x));
	pitch = glm::degrees(asin(-direction.y));

	updateCameraVectors();
}

void Camera::FrameAll(const Vec3 &center, float radius) {
	//Distance to frame object
	float distance = radius / tan(glm::radians(zoom * 0.5f));

	position = center + front * (-distance * 1.5f);

	LookAt(center);
}

void Camera::updateCameraVectors() {
	Vec3 newFront;
	newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	newFront.y = sin(glm::radians(pitch));
	newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(newFront);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}
