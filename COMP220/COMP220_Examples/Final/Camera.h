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
		m_CameraX += xMove / 200.0f;
		m_CameraY -= yMove / 200.0f;

		// Calculate camera target using CameraX and CameraY
		m_CameraTarget = m_CameraPosition + m_CameraDistance * glm::vec3(cos(m_CameraX), tan(m_CameraY), sin(m_CameraX));
		// Normalised camera direction
		m_CameraDirection = normalize(m_CameraTarget - m_CameraPosition);

	};

	void moveCameraForward(float change)
	{
		m_FPScameraPos = m_CameraDirection * change;
		update();
	}

	void moveCameraRight(float change)
	{
		m_FPScameraPos = cross(m_CameraDirection, m_CameraUp) * change;
		update();
	}

	void update()
	{
		m_CameraPosition += m_FPScameraPos;
		m_CameraTarget += m_FPScameraPos;
	}

	void updateViewMatrix()
	{
		m_ViewMatrix = lookAt(m_CameraPosition, m_CameraTarget, m_CameraUp);
	}
	glm::mat4& getViewMatrix()
	{
		return m_ViewMatrix;
	}
	glm::mat4& getProjectionMatrix()
	{
		return m_ProjectionMatrix;
	}
	void setCameraPoisition(float XVal, float YVal, float ZVal)
	{
		m_CameraPosition = glm::vec3(XVal, YVal, ZVal);
	}
	glm::vec3& getCameraPosition()
	{
		return m_CameraPosition;
	}
	void setCameraTarget(float XVal, float YVal, float ZVal)
	{
		m_CameraTarget = glm::vec3(XVal, YVal, ZVal);
	}


private:;
		glm::vec3 m_CameraPosition;
		glm::vec3 m_CameraTarget;
		glm::vec3 m_CameraUp;
		glm::vec3 m_CameraDirection;

		float m_CameraX;
		float m_CameraY;
		float m_CameraDistance;
		
		glm::vec3 m_FPScameraPos;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionMatrix;

};
