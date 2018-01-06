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

	void applyCentralImpulse(float XVal, float YVal, float ZVal)
	{
		m_RigidBody->applyCentralImpulse(btVector3(XVal, YVal, ZVal));
	}

	void makeRigidBody();
	void update(btDiscreteDynamicsWorld*);
	void destroy(btDiscreteDynamicsWorld*);
private:;
	//btDiscreteDynamicsWorld* dynamicsWorld;

		btCollisionShape* m_CollisionShape;
		btTransform m_Transform;
		btVector3 m_Inertia;
		btScalar m_Mass;
		btDefaultMotionState* m_MotionState;
		//btRigidBody::btRigidBodyConstructionInfo m_rbInfo;
		btRigidBody* m_RigidBody;

		//btQuaternion m_Rotation;
};