
#include "kineObject.h"

KinematicObject::KinematicObject()
{

}

KinematicObject::~KinematicObject()
{
}

void KinematicObject::Update(unsigned int dt)
{
	  animator.Update(dt);

	  //update physics object
	  if (physics != NULL)
	  {
		  //update based on model
		  physics->getMotionState()->setWorldTransform(glmToBt(model));
	  }

	  //update keyboard
	  for (int i = 0; i < listener.getSize(); i++)
	  {
		  keyboard(listener.getEvent(i));
	  }

	  //update children
	  for (int i = 0; i < children.size(); i++)
	  {
		  children[i]->setMultiplier(multiplier);
		  children[i]->Update(dt);
	  }
}

void KinematicObject::initPhysics()
{
	setProperties(0,1,0);
	PhysObject::initPhysics();
	physics->setCollisionFlags(physics->getCollisionFlags() |
			btCollisionObject::CF_KINEMATIC_OBJECT);
	physics->setActivationState(DISABLE_DEACTIVATION);
}


void KinematicObject::OnCollisionDetected (PhysObject* hit) {

	cout << "Kinematic object hit another object" << endl;
}
void KinematicObject::OnRaycastHit () {

	cout << "Kinematic object at " << this << " hit by ray" << endl;
}
