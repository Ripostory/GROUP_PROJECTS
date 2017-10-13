#include "physObject.h"

PhysObject::PhysObject()
{
	physics = NULL;
	shape = new btSphereShape(1);
	mass = 0.0f;
}

PhysObject::~PhysObject()
{
	listener.getWorld()->removeCollisionObject(physics);
	delete physics;
	delete shape;
	physics = NULL;
	shape = NULL;
	mass = 0.0f;
}

void PhysObject::Update(unsigned int dt)
{
	  //update physics object
	  if (physics != NULL)
	  {
		  physics->getMotionState()->getWorldTransform(transform);
		  model = btToGlm(transform);
	  }

	  //update children
	  for (int i = 0; i < children.size(); i++)
	  {
		  children[i]->setMultiplier(multiplier);
		  children[i]->Update(dt);
	  }
}

void PhysObject::Begin()
{
	initPhyiscs();
	//start children
	for (int i = 0; i < children.size(); i++)
	{
		children[i]->Begin();
	}
}

void setCollisionMesh(int)
{
	//TODO implement
}

void PhysObject::setCollisionMesh(int box, glm::vec3 size)
{
	delete shape;
	if (box == PHYS_BOX)
	{
		shape = new btBoxShape(glmToBt(size));
	}
	else if (box == PHYS_CYLINDER)
	{
		shape = new btCylinderShape(glmToBt(size));
	}
	else
	{
		cout << "INCORRECT PHYS INITIALIZATION" << endl;
		cout << "Defaulting to sphere" << endl;
		shape = new btSphereShape(1.0f);
	}
}

void PhysObject::setCollisionMesh(int sphere, float radius)
{
	delete shape;
	if (sphere == PHYS_SPHERE)
		shape = new btSphereShape(radius);
	else
	{
		cout << "INCORRECT PHYS INITIALIZATION" << endl;
		cout << "Defaulting to sphere" << endl;
		shape = new btSphereShape(1.0f);
	}
}

void PhysObject::setCollisionMesh(int capCylCone, float radius, float height)
{
	delete shape;
	if (capCylCone == PHYS_CAPSULE)
	{
		shape = new btCapsuleShape(radius, height);
	}
	else if (capCylCone == PHYS_CONE)
	{
		shape = new btConeShape(radius, height);
	}
	else
	{
		cout << "INCORRECT PHYS INITIALIZATION" << endl;
		cout << "Defaulting to sphere" << endl;
		shape = new btSphereShape(1.0f);
	}
}

void PhysObject::setCollisionMesh(int mesh, string filename)
{
	loader readIn;
	obj final;
	btConvexHullShape *meshShape = new btConvexHullShape();
	if (readIn.loadObject(rootDir+filename,final))
	{
		glm::vec3 *vert = NULL;
		//load in collision mesh
		for (int i = 1; i < final.getVerts().size(); i++)
		{
			vert = &final.getVerts()[i].vertex;
			cout <<  final.getIndices()[i] << ": "
					<< vert->x << ", "
					<< vert->y << ", "
					<< vert->z  << endl;
			btVector3 v1(vert->x, vert->y, vert->z);
			meshShape->addPoint(v1, true);
		}

		for (int i = 0; i < final.getIndices().size(); i++)
		{
			cout << final.getIndices()[i] << endl;
		}

		cout << final.getVerts().size() << endl;

		delete shape;
		shape = meshShape;
	}
}

void PhysObject::initPhyiscs()
{
	//base initial position on model matrix
	transform.setIdentity();
	transform.setOrigin(glmToBt(glm::vec3(glm::vec4(0, 0, 0, 1) * glm::transpose(model))));
	mass = 0.1f;

	btVector3 inertia(0,0,0);
	shape->calculateLocalInertia(mass, inertia);

	btDefaultMotionState* objMotionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo objCI(0, NULL, NULL, inertia);
	objCI.m_localInertia = inertia;
	objCI.m_collisionShape = shape;
	objCI.m_motionState = objMotionState;
	objCI.m_mass = mass;
	objCI.m_restitution = 0.8f;

	physics = new btRigidBody(objCI);
	listener.getWorld()->addRigidBody(physics);

}

glm::vec3 PhysObject::btToGlm(btVector3 input)
{
	return glm::vec3(input.getX(), input.getY(), input.getZ());
}

glm::mat4 PhysObject::btToGlm(btTransform matrix)
{
	float *extract;
	matrix.getOpenGLMatrix(extract);
	glm::mat4 final = glm::make_mat4(extract);
	return final;
}

btVector3 PhysObject::glmToBt(glm::vec3 input)
{
	return btVector3(input.x, input.y, input.z);
}
