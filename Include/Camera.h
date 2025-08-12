#pragma once

#include "main.h"

class Camera {
private:
	enum class Movement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

private:
	//* Attributes
	Vec3 position;
	Vec3 front;
	Vec3 up;
	Vec3 right;
	Vec3 worldUp;

	//* Options
	float moveSpeed;
	float mouseSensitivity;
	float zoom;
	float aspectRatio;

	//* Euler angles
	float yaw;
	float pitch;

	//* Constraints
	static constexpr float YAW = -90.0f;
	static constexpr float PITCH = 0.0f;
	static constexpr float SPEED = 2.6f;
	static constexpr float SENSITIVITY = 0.1f;
	static constexpr float ZOOM = 45.0f;

public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		   glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		   float yaw = YAW, float pitch = PITCH);

	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ,
			float yaw, float pitch);
	~Camera();

	Mat4 GetViewMatrix() const;
	Mat4 GetProjectionMatrix() const;

	void ProcessKeyboard(Movement direction, float deltaTime);
	void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);
	void ProcessMousePan(float xOffset, float yOffset);
	void ProcessMouseScroll(float yOffset);

	//* Getters
	Vec3 GetPosition();
	Vec3 GetFront();
	Vec3 GetUp();
	Vec3 GetRight();
	float GetZoom();
	float GetAspectRatio();

	//* Setters

};
