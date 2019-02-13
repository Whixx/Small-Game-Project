#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <string>

class Camera
{
public:
	// Default constructor
	Camera();
	// Assignment constructor
	Camera(const glm::vec3& pos, float fov, float aspect, float zNear, float zFar, glm::vec3 vector);
	virtual ~Camera();

	// These functions are for camera control and movement
	void SetCameraPosition(glm::vec3 camPos);
	void SetOldCameraPosition(glm::vec3 camPos);
	void SetForwardVector(glm::vec3 forwardVector);

	glm::vec3 GetForwardVector();
	glm::vec3 GetCameraPosition();
	glm::vec3 GetOldCameraPosition();
	glm::vec3 GetStartCameraPosition();
	glm::vec3 GetStartForwardVector();
	glm::vec3 GetUpVector();
	glm::vec3 GetRightVector();
	glm::vec3 GetRotateAround();
	glm::mat4 GetViewProjection() const;
	glm::vec2 GetOldMousePosition();
	glm::vec2 GetMouseDelta();
	float GetPitch();
	float GetYaw();
	float GetRotationalSpeed();

	void SetProjectionMatrix(glm::mat4 matrix);
	void SetUpVector(glm::vec3 vector);
	void SetViewMatrix(glm::mat4 matrix);
	void SetRotateAround(glm::vec3 rotate);
	void SetStartCameraPosition(glm::vec3 position);
	void SetStartForwardVector(glm::vec3 vector);
	void SetOldMousePosition(glm::vec2 oldPos);
	void SetMouseDelta(glm::vec2 mouseDelta);

	void UpdateViewMatrix();
	void UpdateRotation();
private:
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
	glm::vec3 cameraPosition;
	glm::vec3 oldCameraPosition;
	glm::vec3 forwardVector;	// Viewvector // frontal view
	glm::vec3 upVector;
	
	glm::vec3 startCameraPosition;
	glm::vec3 startForwardVector;
	
	// Used for mouseUpdate
	glm::vec2 oldMousePosition;
	glm::vec2 mouseDelta;

	float yaw;
	float pitch;

	// The vector which we rotate around during the vertical camera rotation.
	glm::vec3 rotateAround;
	const float rotationalSpeed = 0.0025f;
	const float movementSpeed = 0.2f;
};

#endif