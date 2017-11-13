#include "plunger.h"

Plunger::Plunger()
{
	isActive = false;
	key = 'p';
}

Plunger::Plunger(char k) : PhysObject(Layer_Plunger, Layer_Ball)
{
	  loadModel("models/plunger.obj");
	  loadTexture("textures/paddle.png");
	  setProperties(100.0, 0.5, 1.1);
	  setMeshCollider(Physics_Mesh_Hull, "models/plunger.obj");
	  isActive = false;
	  key = 'z' - 'a';
	  translate(glm::vec3(6.1, 0, -20.9));


		initPhysics();

		btTransform tr = transform;

		btSliderConstraint* slider = new btSliderConstraint (
			*physics,
			tr,
			true
		);
	
		slider -> setLowerLinLimit (2.0);
		slider -> setUpperLinLimit (7.0);

		listener.getWorld () -> addConstraint (slider);
}

Plunger::~Plunger()
{

}

void Plunger::keyboard(eventType event)
{
	if (event.eventVer == SDL_KEYDOWN)
		{
			if (event.key == SDLK_a + key) {
				isActive = true;
			}
		}
	else if (event.eventVer == SDL_KEYUP)
	{
		if (event.key == SDLK_a + key) {
			isActive = false;
		}
	}
}

void Plunger::Update(unsigned int dt)
{
	  //update keyboard
	for (int i = 0; i < listener.getSize(); i++)
	{
		  keyboard(listener.getEvent(i));
	}

		if (isActive) {

			cout << "FIRE!" << endl;
			physics -> applyCentralImpulse (btVector3 (-20, 0, 0));
		}
		else if (!isActive) {

			physics -> applyCentralImpulse (btVector3 (2, 0, 0));
			cout << "WAIT!" << endl;
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
