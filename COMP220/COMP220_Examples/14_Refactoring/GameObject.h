#pragma once

#include <vector>

#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Mesh.h"
#include "Model.h"
#include "Texture.h"
#include "Shader.h"

class GameObject
{
public:
		GameObject();
	   ~GameObject();

	   void setPosition(const glm::vec3& position)
	   {
		   m_Position = position;
	   };
	   glm::vec3& getPosition()
	   {
		   return m_Position;
	   };

	   void setScale(const glm::vec3& scale)
	   {
		   m_Scale = scale;
	   };
	   glm::vec3& getScale()
	   {
		   return m_Scale;
	   };

	   void setRotation(const glm::vec3& rotation)
	   {
		   m_Rotation = rotation;
	   };
	   glm::vec3& getRotation()
	   {
		   return m_Rotation;
	   };

	   glm::mat4& getModelMatrix()
	   {
		   return m_ModelMatrix;
	   };

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
	   };
	   float getSpecularPower()
	   {
		   return m_SpecularPower;
	   };

	   GLuint getShaderProgramID()
	   {
		   return m_ShaderProgramID;
	   }

	   void loadMesh(const std::string& filename);

	   void loadDiffuseMap(const std::string& filename);

	   void loadShaderProgram(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);

	   void destroy();

	   void update();
	   void preRender();
	   void render();
private:;
	std::vector<Mesh*> m_Meshes;

	//transform class
	glm::vec3 m_Position;
	glm::vec3 m_Scale;
	glm::vec3 m_Rotation;
	glm::mat4 m_ModelMatrix;

	//material class
	GLuint m_DiffuseMapID;
	glm::vec4 m_AmbientMaterialColour;
	glm::vec4 m_DiffuseMaterialColour;
	glm::vec4 m_SpecularMaterialColour;
	float m_SpecularPower;
	GLint m_ShaderProgramID;

};

// m_ = member varible