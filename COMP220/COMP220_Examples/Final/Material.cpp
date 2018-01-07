#include "Material.h"

Material::Material()
{

	m_DiffuseMapID = 0;
	m_AmbientMaterialColour = glm::vec4(0.4f, 0.4f, 0.4f, 1.0f);
	m_DiffuseMaterialColour = glm::vec4(0.6f, 0.6f, 0.6f, 1.0f);
	m_SpecularMaterialColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_SpecularPower = 25.0f;

	m_ShaderProgramID = 0;
}

Material::~Material()
{
}

void Material::render()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_DiffuseMapID);

	glUseProgram(m_ShaderProgramID);

	GLint textureLocation = glGetUniformLocation(m_ShaderProgramID, "baseTexture");

	GLint ambientMaterialColourLocation = glGetUniformLocation(m_ShaderProgramID, "ambientMaterialColour");
	GLint diffuseMaterialColourLocation = glGetUniformLocation(m_ShaderProgramID, "diffuseMaterialColour");
	GLint specularMaterialColourLocation = glGetUniformLocation(m_ShaderProgramID, "diffuseMaterialColour");
	GLint specularPowerLocation = glGetUniformLocation(m_ShaderProgramID, "specularPower");

	glUniform1i(textureLocation, 0);
	glUniform4fv(ambientMaterialColourLocation, 1, value_ptr(m_AmbientMaterialColour));
	glUniform4fv(diffuseMaterialColourLocation, 1, value_ptr(m_DiffuseMaterialColour));
	glUniform4fv(specularMaterialColourLocation, 1, value_ptr(m_SpecularMaterialColour));
	glUniform1f(specularPowerLocation, m_SpecularPower);
}