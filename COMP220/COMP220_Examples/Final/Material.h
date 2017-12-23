#pragma once

#include <glm\glm.hpp>

#include "Shader.h"

class Material
{
public:
	Material();
	~Material();

	void setAmbientMaterialColour(const glm::vec4& ambientMaterialColour)
	{
		m_AmbientMaterialColour = ambientMaterialColour;
	};
	glm::vec4& getAmbientMaterialColour()
	{
		return m_AmbientMaterialColour;
	};

	void setDiffuseMaterialColour(const glm::vec4& diffuseMaterialColour)
	{
		m_DiffuseMaterialColour = diffuseMaterialColour;
	};
	glm::vec4& getDiffuseMaterialColour()
	{
		return m_DiffuseMaterialColour;
	};

	void setSpecularMaterialColour(const glm::vec4& specularMaterialColour)
	{
		m_SpecularMaterialColour = specularMaterialColour;
	};
	glm::vec4& getSpecularMaterialColour()
	{
		return m_SpecularMaterialColour;
	};

private:;
GLuint m_DiffuseMapID;
glm::vec4 m_AmbientMaterialColour;
glm::vec4 m_DiffuseMaterialColour;
glm::vec4 m_SpecularMaterialColour;
float m_SpecularPower;
GLint m_ShaderProgramID;

};