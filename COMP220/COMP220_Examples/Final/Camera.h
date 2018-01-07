#pragma once


#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>


class Camera
{
public:
	Camera();
	~Camera();

	void setRotation(float xMove, float yMove)
	{
		CameraX += xMove / 200.0f;
		CameraY -= yMove / 200.0f;

		// Calculate camera target using CameraX and CameraY
		cameraTarget = cameraPosition + CameraDistance * glm::vec3(cos(CameraX), tan(CameraY), sin(CameraX));
		// Normalised camera direction
		cameraDirection = normalize(cameraTarget - cameraPosition);

	};

	void moveCameraForward(float change)
	{
		FPScameraPos = cameraDirection * change;
		update();
	}

	void moveCameraRight(float change)
	{
		FPScameraPos = cross(cameraDirection, cameraUp) * change;
		update();
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
	glm::mat4& getViewMatrix()
	{
		return viewMatrix;
	}
	glm::mat4& getProjectionMatrix()
	{
		return projectionMatrix;
	}
	glm::vec3& getCameraPosition()
	{
		return cameraPosition;
	}


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
		glm::mat4 projectionMatrix;

};
