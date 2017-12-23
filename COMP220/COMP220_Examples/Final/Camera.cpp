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

	m_ShaderProgramID = 0;
}

Camera::~Camera()
{
}

void Camera::test()
{

	GLint viewMatrixLocation = glGetUniformLocation(m_ShaderProgramID, "viewMatrix");
	GLint projectionMatrixLocation = glGetUniformLocation(m_ShaderProgramID, "projectionMatrix");
	GLint camerPositionLocation = glGetUniformLocation(m_ShaderProgramID, "cameraPosition");

	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, value_ptr(viewMatrix));
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, value_ptr(projectionMatrix));
	glUniform3fv(camerPositionLocation, 1, value_ptr(cameraPosition));

}
