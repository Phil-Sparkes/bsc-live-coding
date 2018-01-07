#include "GameObject.h"

GameObject::GameObject()
{
	m_Transform = new ObjectTransform();
	m_Physics = new PhysicsObject();
	m_Material = new Material();

	m_HasPhysics = false;
}

GameObject::~GameObject()
{
}


void GameObject::destroy()
{
	m_Material->destroy();

}


