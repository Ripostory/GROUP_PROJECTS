
#include "bumper.h"

Bumper::Bumper() { }

Bumper::Bumper(glm::vec3 position) : PhysObject(Layer_Plunger, Layer_Ball)
{

	  loadModel("models/cylBumper.obj");
	  loadTexture("textures/cylBumper.png");
	  
		setBoxCollider(Physics_Mesh_Cylinder, glm::vec3(1.5, 2, 1.5));
	 	setProperties(0,0,2);
	  translate(position);



		initPhysics();
}

Bumper::~Bumper()
{

}

void Bumper::keyboard(eventType event)
{

}

void Bumper::OnCollisionDetected (PhysObject* hit) {

	GUI::getInstance () -> ChangeScore (25);
}

void Bumper::Update(unsigned int dt)
{
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
