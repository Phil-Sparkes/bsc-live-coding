#pragma once

#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>

class ObjectTransform
{
public:
	ObjectTransform();
	~ObjectTransform();

	void setPosition(float xVal, float yVal, float zVal)
	{
		m_Position = glm::vec3(xVal, yVal, zVal);
	};
	glm::vec3& getPosition()
	{
		return m_Position;
	};

	void setScale(float xVal, float yVal, float zVal)
	{
		m_Scale = glm::vec3(xVal, yVal, zVal);
	};
	glm::vec3& getScale()
	{
		return m_Scale;
	};

	void setRotation(float xVal, float yVal, float zVal)
	{
		m_Rotation = glm::vec3(xVal, yVal, zVal);
	};
	glm::vec3& getRotation()
	{
		return m_Rotation;
	};

	glm::mat4& getModelMatrix()
	{
		return m_ModelMatrix;
	};

	void update();

private:;
		glm::vec3 m_Position;
		glm::vec3 m_Scale;
		glm::vec3 m_Rotation;
		glm::mat4 m_ModelMatrix;
};