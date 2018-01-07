#pragma once

#include "ObjectTransform.h"
#include "PhysicsObject.h"
#include "Material.h"

class GameObject
{
public:
		GameObject();
	   ~GameObject();

	   void setObjectTransform(ObjectTransform * transform)
	   {
		   m_Transform = transform;
	   }
	   ObjectTransform& getObjectTransform()
	   {
		   return * m_Transform;
	   }
	   void setObjectPhysics(PhysicsObject * physics)
	   {
		   m_Physics = physics;
	   }
	   PhysicsObject& getObjectPhysics()
	   {
		   return * m_Physics;
	   }
	   void setMaterial(Material * material)
	   {
		   m_Material = material;
	   }
	   Material& getMaterial()
	   {
		   return *m_Material;
	   }

	   void setHasPhysics(bool has)
	   {
		   m_HasPhysics = has;
	   }
	   bool getHasPhysics()
	   {
		   return m_HasPhysics;
	   }

	   void destroy();
	   void update();
private:;


	//material class

	ObjectTransform * m_Transform;
	PhysicsObject * m_Physics;
	Material * m_Material;

	bool m_HasPhysics;
};

// m_ = member varible