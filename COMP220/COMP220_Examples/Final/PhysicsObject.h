#pragma once

#include "btBulletDynamicsCommon.h"

class PhysicsObject
{
public:
	PhysicsObject();
	~PhysicsObject();

	void setCollisionShape(int XVal, int YVal, int ZVal)
	{
		m_CollisionShape = new btBoxShape(btVector3(XVal, YVal, ZVal));
	};
	void setObjectOrigin(int XVal, int YVal, int ZVal)
	{
	m_Transform.setOrigin(btVector3(XVal, YVal, ZVal));
	}
	
	btVector3& getObjectOrigin()
	{
		return m_Transform.getOrigin();
	}
	btQuaternion getObjectRotation()
	{
		return m_Transform.getRotation();
	}

	void setObjectMass(float value)
	{
		m_Mass = value;
	}
	void setObjectInertia(int XVal, int YVal, int ZVal)
	{
		m_Inertia = btVector3(XVal, YVal, ZVal);
	}

	void addRigidBody(btDiscreteDynamicsWorld* world)
	{
		world->addRigidBody(m_RigidBody);
	}
	

	void setObjectSpeed(float XVal, float YVal, float ZVal)
	{
		m_RigidBody->activate();
		m_RigidBody->setLinearVelocity(btVector3(XVal, YVal, ZVal));
	}


	float getSpeed()
	{
		return m_speed;
	}

	void randomiseSpeed();
	void makeRigidBody();
	void update(btDiscreteDynamicsWorld*);
	void destroy(btDiscreteDynamicsWorld*);
	void setTransform(float XVal, float YVal, float ZVal);
private:;
		btCollisionShape* m_CollisionShape;
		btTransform m_Transform;
		btVector3 m_Inertia;
		btScalar m_Mass;
		btDefaultMotionState* m_MotionState;
		btRigidBody* m_RigidBody;
		float m_speed;
};