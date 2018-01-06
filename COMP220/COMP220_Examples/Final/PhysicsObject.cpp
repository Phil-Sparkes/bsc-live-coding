#include "PhysicsObject.h"

PhysicsObject::PhysicsObject()
{
	m_CollisionShape = new btBoxShape(btVector3(2, 2, 2));

	m_Transform.setIdentity();

	m_Inertia = btVector3(0, 0, 0);
	m_Mass = btScalar(0.f);
}

PhysicsObject::~PhysicsObject()
{
}


void PhysicsObject::makeRigidBody()
{
	m_CollisionShape->calculateLocalInertia(m_Mass, m_Inertia);

	m_MotionState = new btDefaultMotionState(m_Transform);
	btRigidBody::btRigidBodyConstructionInfo m_rbInfo(m_Mass, m_MotionState, m_CollisionShape, m_Inertia);
	m_RigidBody = new btRigidBody(m_rbInfo);
}


void PhysicsObject::update(btDiscreteDynamicsWorld* world)
{

	m_Transform = m_RigidBody->getWorldTransform();

}

void PhysicsObject::destroy(btDiscreteDynamicsWorld* world)
{
	world->removeRigidBody(m_RigidBody);

	delete m_CollisionShape;
	delete m_RigidBody->getMotionState();
	delete m_RigidBody;
}