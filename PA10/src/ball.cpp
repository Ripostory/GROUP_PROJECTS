#include "ball.h"

Ball::Ball (glm::vec3 position) : PhysObject(Layer_All, Layer_All) {

	loadModel ("models/planet.obj");
	setSphereCollider(Physics_Mesh_Sphere, 1);
	translate (position);
	setProperties (0.3f, 0.5f, 0.5f);

	initPhysics ();
}

void Ball::keyboard(eventType) { }

void Ball::Update(unsigned int dt) { 

		//update keyboard
		for (int i = 0; i < listener.getSize(); i++)
		{
				keyboard(listener.getEvent(i));
		}

	  //update physics object
	  if (physics != NULL)
	  {
		  physics->getMotionState()->getWorldTransform(transform);
		  model = btToGlm(transform);
		  //update scale
		  model *= mscale;
	  }

	  //update children
	  for (int i = 0; i < children.size(); i++)
	  {
		  children[i]->setMultiplier(multiplier);
		  children[i]->Update(dt);
	  }
}



