#include "Camera.h"

Camera::Camera(const glm::vec3& pos, float fov, float aspect, float zNear, float zFar, glm::vec3 vector)
{
	this->projectionMatrix = glm::perspective(fov, aspect, zNear, zFar);
	this->cameraPosition = pos;
	this->forwardVector = vector;
	this->walkingVector = vector;
	this->upVector = glm::vec3(0, 1, 0);
	viewMatrix = glm::lookAt(this->cameraPosition, this->cameraPosition + this->forwardVector, this->upVector);

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

glm::vec3 Camera::getForwardVector()
{
	return this->forwardVector;
}

// Sets
void Camera::setCameraPosition(glm::vec3 camPos)
{
	this->cameraPosition = camPos;
}

void Camera::setForwardVector(glm::vec3 forwardVector)
{
	this->forwardVector = forwardVector;
}

void Camera::moveForward(float elapsedTime)
{
	this->cameraPosition += this->movementSpeed * this->walkingVector * elapsedTime;
}

void Camera::moveBackward(float elapsedTime)
{
	this->cameraPosition -= this->movementSpeed * this->walkingVector * elapsedTime;
}

void Camera::moveRight(float elapsedTime)
{
	this->cameraPosition += this->movementSpeed * this->rotateAround * elapsedTime;
}

void Camera::moveLeft(float elapsedTime)
{
	this->cameraPosition -= this->movementSpeed * this->rotateAround * elapsedTime;
}

void Camera::moveUp(float elapsedTime)
{
	this->cameraPosition += this->movementSpeed * this->upVector * elapsedTime;
}

void Camera::moveDown(float elapsedTime)
{
	this->cameraPosition -= this->movementSpeed * this->upVector * elapsedTime;
}

void Camera::updateCamera(const glm::vec2& newMousePosition, float elapsedTime)
{
	// Get mouse delta vector, how much the mouse has moved
	this->mouseDelta = newMousePosition - this->oldMousePosition;
	// if the mouseDelta is to far away, the camera will jump to unpredicted areas.
	if (glm::length(this->mouseDelta) < 50.0f)
	{
		//Update the horizontal view
		this->forwardVector = glm::mat3(glm::rotate(-mouseDelta.x *this->rotationalSpeed, this->upVector)) * this->forwardVector;
		walkingVector = glm::mat3(glm::rotate(-mouseDelta.x *this->rotationalSpeed, this->upVector)) * walkingVector;

		//Update the vertical view
		this->rotateAround = glm::cross(this->forwardVector, this->upVector);
		this->forwardVector = glm::mat3(glm::rotate(-mouseDelta.y *this->rotationalSpeed, this->rotateAround)) * this->forwardVector;
	}
	this->oldMousePosition = newMousePosition;
}

void Camera::updateViewMatrix()
{
	this->viewMatrix = glm::lookAt(this->cameraPosition, this->cameraPosition + this->forwardVector, this->upVector);
}

