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
glm::mat4 Camera::getViewProjection() const
{
	return this->projectionMatrix * this->viewMatrix;
}

glm::vec2 Camera::getOldMousePosition()
{
	return this->oldMousePosition;
}

glm::vec2 Camera::getMouseDelta()
{
	return mouseDelta;
}

float Camera::getRotationalSpeed()
{
	return this->rotationalSpeed;
}

glm::vec3 Camera::getCameraPosition()
{
	return this->cameraPosition;
}

glm::vec3 Camera::getStartCameraPosition()
{
	return this->startCameraPosition;
}

glm::vec3 Camera::getStartForwardVector()
{
	return this->startForwardVector;
}

glm::vec3 Camera::getUpVector()
{
	return this->upVector;
}

glm::vec3 Camera::getRightVector()
{
	return glm::cross(this->forwardVector, this->upVector);
}

glm::vec3 Camera::getRotateAround()
{
	return this->rotateAround;
}

glm::vec3 Camera::getForwardVector()
{
	return this->forwardVector;
}

// Sets
void Camera::setCameraPosition(glm::vec3 camPos)
{
	this->cameraPosition = camPos;
}

void Camera::setUpVector(glm::vec3 vector)
{
	this->upVector = vector;
}

void Camera::setViewMatrix(glm::mat4 matrix)
{
	this->viewMatrix = matrix;
}

void Camera::setRotateAround(glm::vec3 rotate)
{
	this->rotateAround = rotate;
}

void Camera::setStartCameraPosition(glm::vec3 position)
{
	this->startCameraPosition = position;
}

void Camera::setStartForwardVector(glm::vec3 vector)
{
	this->startForwardVector = vector;
}

void Camera::setOldMousePosition(glm::vec2 oldPos)
{
	this->oldMousePosition = oldPos;
}

void Camera::setMouseDelta(glm::vec2 mouseDelta)
{
	this->mouseDelta = mouseDelta;
}

void Camera::setForwardVector(glm::vec3 forwardVector)
{
	this->forwardVector = forwardVector;
}

void Camera::setProjectionMatrix(glm::mat4 matrix)
{
	this->projectionMatrix = matrix;
}

void Camera::updateViewMatrix()
{
	this->viewMatrix = glm::lookAt(this->cameraPosition, this->cameraPosition + this->forwardVector, this->upVector);
}

