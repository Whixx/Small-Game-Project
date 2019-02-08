#include "Camera.h"

Camera::Camera()
{
}

Camera::Camera(const glm::vec3& pos, float fov, float aspect, float zNear, float zFar, glm::vec3 vector)
{
	this->projectionMatrix = glm::perspective(fov, aspect, zNear, zFar);
	this->cameraPosition = pos;
	this->forwardVector = vector;
	this->walkingVector = vector;
	this->upVector = glm::vec3(0, 1, 0);
	this->viewMatrix = glm::lookAt(this->cameraPosition, this->cameraPosition + this->forwardVector, this->upVector);

	// Init rotateAround
	this->rotateAround = glm::cross(this->forwardVector, this->upVector);

	// Used to locate the start position and view direction of the camera if the user gets lost.
	this->startCameraPosition = pos;
	this->startForwardVector = this->forwardVector;
}

Camera::~Camera()
{
}

// Gets
glm::mat4 Camera::GetViewProjection() const
{
	return this->projectionMatrix * this->viewMatrix;
}

glm::vec2 Camera::GetOldMousePosition()
{
	return this->oldMousePosition;
}

glm::vec2 Camera::GetMouseDelta()
{
	return this->mouseDelta;
}

float Camera::GetRotationalSpeed()
{
	return this->rotationalSpeed;
}

glm::vec3 Camera::GetCameraPosition()
{
	return this->cameraPosition;
}

glm::vec3 Camera::GetStartCameraPosition()
{
	return this->startCameraPosition;
}

glm::vec3 Camera::GetStartForwardVector()
{
	return this->startForwardVector;
}

glm::vec3 Camera::GetUpVector()
{
	return this->upVector;
}

glm::vec3 Camera::GetRightVector()
{
	return glm::cross(this->forwardVector, this->upVector);
}

glm::vec3 Camera::GetRotateAround()
{
	return this->rotateAround;
}

glm::vec3 Camera::GetForwardVector()
{
	return this->forwardVector;
}

// Sets
void Camera::SetCameraPosition(glm::vec3 camPos)
{
	this->cameraPosition = camPos;
}

void Camera::SetUpVector(glm::vec3 vector)
{
	this->upVector = vector;
}

void Camera::SetViewMatrix(glm::mat4 matrix)
{
	this->viewMatrix = matrix;
}

void Camera::SetRotateAround(glm::vec3 rotate)
{
	this->rotateAround = rotate;
}

void Camera::SetStartCameraPosition(glm::vec3 position)
{
	this->startCameraPosition = position;
}

void Camera::SetStartForwardVector(glm::vec3 vector)
{
	this->startForwardVector = vector;
}

void Camera::SetOldMousePosition(glm::vec2 oldPos)
{
	this->oldMousePosition = oldPos;
}

void Camera::SetMouseDelta(glm::vec2 mouseDelta)
{
	this->mouseDelta = mouseDelta;
}

void Camera::SetForwardVector(glm::vec3 forwardVector)
{
	this->forwardVector = forwardVector;
}

void Camera::SetProjectionMatrix(glm::mat4 matrix)
{
	this->projectionMatrix = matrix;
}

void Camera::UpdateViewMatrix()
{
	this->viewMatrix = glm::lookAt(this->cameraPosition, this->cameraPosition + this->forwardVector, this->upVector);
}
