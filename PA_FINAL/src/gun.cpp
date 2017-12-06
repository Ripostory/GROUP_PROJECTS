#include "gun.h"

Gun::Gun()
{
	Cam = NULL;
}

Gun::Gun(camera *cam)
{
	Cam = cam;
	loadModel("newBoard.obj");
	loadNormal("cleanNormal.png");

	spawnTracer();
}

Gun::~Gun()
{

}

void Gun::Update(unsigned int dt)
{
	//move to camera
	translate(glm::vec3(0,-8,0));
	model = mrotate * mtranslate  * glm::inverse(Cam->GetView());
	KinematicObject::Update(dt);

	//clean tracers
	//assume the most recent tracer is the first tracer to despawn
	if (children.size() != 0)
	{
		if (!children.front()->animator.isPending())
		{
			delete *children.begin();
			children.erase(children.begin());
		}
	}
}

void Gun::keyboard(eventType event)
{
	if (event.eventVer == SDL_KEYDOWN)
	{
		if (event.key == SDLK_a)
			spawnTracer();
	}
}

void Gun::spawnTracer()
{
	glm::mat4 view = Cam->GetView();
	view = glm::inverse(view);
	Object *tracer = new Object();
	tracer->loadModel("planet.obj");
	tracer->loadNormal("cleanNormal.png");
	tracer->translate(glm::vec3(Cam->GetPosition()));
	tracer->lerpTo(glm::vec3(view[2][0], view[2][1], view[2][2])*-10000.0f, 10);
	addChild(tracer);
}
