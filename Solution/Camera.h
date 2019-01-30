#ifndef CAMERA_H
#define CAMERA_H

#include <glm\glm.hpp>
#include <glm/gtx/transform.hpp>
class Camera
{
public:
	Camera(const glm::vec3& pos, float fov, float aspect, float zNear, float zFar);

	glm::mat4 getViewProjection() const;
	virtual ~Camera();

	// These functions are for camera control and movement
	void mouseUpdate(const glm::vec2& newMousePosition);
	void setCameraPosition(glm::vec3 camPos);
	void setForwardVector(glm::vec3 forwardVector);

	glm::vec3 getForwardVector();
	glm::vec3 getCameraPosition();
	glm::vec3 getStartCameraPosition();
	glm::vec3 getStartForwardVector();
	glm::vec3 getUpVector();
	glm::vec3 getRightVector();


	void moveForward();
	void moveBackward();
	void moveRight();
	void moveLeft();
	void moveUp();
	void moveDown();

	void updateViewMatrix();
private:
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
	glm::vec3 cameraPosition;
	glm::vec3 forwardVector;	// Viewvector // frontal view
	glm::vec3 upVector;

	glm::vec3 startCameraPosition;
	glm::vec3 startForwardVector;

	// Used for mouseUpdate
	glm::vec2 oldMousePosition;
	glm::vec2 mouseDelta;
	// The vector which we rotate around during the vertical camera rotation.
	glm::vec3 rotateAround;
	const float rotationalSpeed = 0.01f;
	const float movementSpeed = 0.2f;

};

#endif