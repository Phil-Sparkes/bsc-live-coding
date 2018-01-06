#include "ObjectTransform.h"

ObjectTransform::ObjectTransform()
{
	m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_Scale = glm::vec3(1.0f, 1.0f, 1.0f);
	m_Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	m_ModelMatrix = glm::mat4(1.0f);
}

ObjectTransform::~ObjectTransform()
{
}


void ObjectTransform::update()
{
	glm::mat4 translationMatrix = glm::translate(m_Position);
	glm::mat4 scaleMatrix = glm::scale(m_Scale);
	glm::mat4 rotationMatrix = glm::rotate(m_Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f))*
		glm::rotate(m_Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f))*
		glm::rotate(m_Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

	m_ModelMatrix = translationMatrix*rotationMatrix*scaleMatrix;

}