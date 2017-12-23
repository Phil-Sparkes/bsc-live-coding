#pragma once

#include <vector>

#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Mesh.h"
#include "Model.h"
#include "Texture.h"
#include "Shader.h"

class Camera
{
public:
	Camera();
	~Camera();

	void setRotation(float xMove, float yMove)
	{
		CameraX += xMove / 200.0f;
		CameraY += yMove / 200.0f;

		if (CameraY > 150.0f) CameraY = 150.0f; else if (CameraY < -150.0f) CameraY = -150.0f;
		// Calculate camera target using CameraX and CameraY
		cameraTarget = cameraPosition + CameraDistance * glm::vec3(cos(CameraX), tan(CameraY), sin(CameraX));
		// Normalised camera direction
		cameraDirection = normalize(cameraTarget - cameraPosition);

	};

	void moveCamera1(float change)
	{
		FPScameraPos = cameraDirection * change;
	}

	void moveCamera2(float change)
	{
		FPScameraPos = cross(cameraDirection, cameraUp) * change;
	}

	void update()
	{
		cameraPosition += FPScameraPos;
		cameraTarget += FPScameraPos;
	}

	void updateViewMatrix()
	{
		viewMatrix = lookAt(cameraPosition, cameraTarget, cameraUp);
	}

	void test();

private:;
		// Camera set up
		glm::vec3 cameraPosition;
		glm::vec3 cameraTarget;
		glm::vec3 cameraUp;
		glm::vec3 cameraDirection;

		float CameraX = 0.0f;
		float CameraY = 0.0f;
		float CameraDistance = (float)(cameraTarget - cameraPosition).length();
		glm::vec3 FPScameraPos;

		glm::mat4 viewMatrix;
		//mat4 modelMatrix = translationMatrix*rotationMatrix*scaleMatrix;
		glm::mat4 projectionMatrix;

		GLint m_ShaderProgramID;

};
