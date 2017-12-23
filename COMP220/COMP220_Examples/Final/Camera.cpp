#include "Camera.h"

Camera::Camera()
{
	// Camera set up
	cameraPosition = glm::vec3(0.0f, 0.0f, -5.0f);
	cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);

	CameraX = 0.0f;
	CameraY = 0.0f;
	CameraDistance = (float)(cameraTarget - cameraPosition).length();
	FPScameraPos = glm::vec3(0.0f);

	viewMatrix = lookAt(cameraPosition, cameraTarget, cameraUp);
	projectionMatrix = glm::perspective(glm::radians(90.0f), float(800 / 800), 0.1f, 100.0f);

}

Camera::~Camera()
{
}


