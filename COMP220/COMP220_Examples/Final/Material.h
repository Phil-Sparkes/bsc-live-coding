#pragma once

#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Mesh.h"
#include "Model.h"
#include "Texture.h"
#include "Shader.h"


class Material
{
public:
	Material();
	~Material();

	GLuint getShaderProgramID()
	{
		return m_ShaderProgramID;
	}
	GLuint getDiffuseMapID()
	{
		return m_DiffuseMapID;
	}

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
	
	void setSpecularPower(float power)
	{
		m_SpecularPower = power;
	}
	float getSpecularPower()
	{
		return m_SpecularPower;
	}

	void loadMesh(const std::string& filename);

	void loadDiffuseMap(const std::string& filename);

	void loadShaderProgram(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);

	void render();
	void preRender();
	void destroy();
private:;
std::vector<Mesh*> m_Meshes;

GLuint m_DiffuseMapID;

glm::vec4 m_AmbientMaterialColour;
glm::vec4 m_DiffuseMaterialColour;
glm::vec4 m_SpecularMaterialColour;
float m_SpecularPower;

GLint m_ShaderProgramID;
};