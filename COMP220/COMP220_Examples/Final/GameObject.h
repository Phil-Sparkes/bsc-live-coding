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


	//material class
	GLuint m_DiffuseMapID;
	glm::vec4 m_AmbientMaterialColour;
	glm::vec4 m_DiffuseMaterialColour;
	glm::vec4 m_SpecularMaterialColour;
	float m_SpecularPower;
	GLint m_ShaderProgramID;
};

// m_ = member varible