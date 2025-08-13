#pragma once

#include "main.h"
#include "Object.h"

#define NEAR 0.1f
#define FAR 1000.0f

class Camera {
public:
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
	float roll;

	//* Constraints
	static constexpr float YAW = -90.0f;
	static constexpr float PITCH = 0.0f;
	static constexpr float SPEED = 2.6f;
	static constexpr float SENSITIVITY = 0.5f;
	static constexpr float ZOOM = 45.0f;

public:
	Camera(Vec3 position = Vec3(0.0f, 0.0f, 0.0f),
		   Vec3 up = Vec3(0.0f, 1.0f, 0.0f),
		   float yaw = YAW, float pitch = PITCH);

	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ,
			float yaw, float pitch);
	~Camera() = default;

	Mat4 GetViewMatrix() const;
	Mat4 GetProjectionMatrix() const;

	void ProcessKeyboard(Movement direction, float deltaTime);
	void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);
	void ProcessMousePan(float xOffset, float yOffset);
	void ProcessMouseScroll(float yOffset);

	//* Getters
	Vec3 GetPosition() const { return position; }
	Vec3 GetFront() const { return front; }
	Vec3 GetUp() const { return up; }
	Vec3 GetRight() const { return right; }
	float GetZoom() const { return zoom; }
	float GetAspectRatio() const { return aspectRatio; }
	float GetYaw() const { return yaw; }
	float GetPitch() const { return pitch; }
	float GetRoll() const { return roll; }

	//* Setters
	void SetPosition(const Vec3 &pos) { position = pos; }
	void SetAspectRatio(float ratio) { aspectRatio = ratio; }

	//* Camera Controls
	void OrbitAround(const Vec3 &target, float xOffset, float yOffset);
	void LookAt(const Vec3 &target);
	void FrameAll(const Vec3 &center, float radius);

private:
	void updateCameraVectors();
};
