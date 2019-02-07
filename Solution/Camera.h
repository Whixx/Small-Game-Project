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
	void setCameraPosition(glm::vec3 camPos);
	void setForwardVector(glm::vec3 forwardVector);

	glm::vec3 getForwardVector();
	glm::vec3 getCameraPosition();
	glm::vec3 getStartCameraPosition();
	glm::vec3 getStartForwardVector();
	glm::vec3 getUpVector();
	glm::vec3 getRightVector();
	glm::vec3 getRotateAround();
	glm::mat4 getViewProjection() const;
	glm::vec2 getOldMousePosition();
	glm::vec2 getMouseDelta();
	float getRotationalSpeed();

	void setProjectionMatrix(glm::mat4 matrix);
	void setUpVector(glm::vec3 vector);
	void setViewMatrix(glm::mat4 matrix);
	void setRotateAround(glm::vec3 rotate);
	void setStartCameraPosition(glm::vec3 position);
	void setStartForwardVector(glm::vec3 vector);
	void setOldMousePosition(glm::vec2 oldPos);
	void setMouseDelta(glm::vec2 mouseDelta);

	void updateViewMatrix();
private:
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
	glm::vec3 cameraPosition;
	glm::vec3 forwardVector;	// Viewvector // frontal view
	glm::vec3 upVector;
	glm::vec3 walkingVector;
	
	glm::vec3 startCameraPosition;
	glm::vec3 startForwardVector;
	
	// Used for mouseUpdate
	glm::vec2 oldMousePosition;
	glm::vec2 mouseDelta;

	// The vector which we rotate around during the vertical camera rotation.
	glm::vec3 rotateAround;
	const float rotationalSpeed = 0.25f;
	const float movementSpeed = 0.2f;
};

#endif