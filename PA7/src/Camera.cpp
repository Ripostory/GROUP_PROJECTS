
#include "Camera.h"

camera::camera()
{
    orbit = 0.0f;
    xPos = 0.0f;
    yPos = 0.0f;
    lookAt = glm::vec4(0.0f);
    distance = -25.0f;
    height = 8.0f;
    parent = NULL;
}

camera::~camera()
{

}

bool camera::Initialize(int w, int h)
{
  //--Init the view and projection matrices
  //  if you will be having a moving camera the view matrix will need to more dynamic
  //  ...Like you should update it before you render more dynamic 
  //  for this project having them static will be fine
  view = glm::lookAt( glm::vec3(0.0, 8.0, -16.0), //Eye Position
                      glm::vec3(0.0, 0.0, 0.0), //Focus point
                      glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up

  projection = glm::perspective( 45.0f, //the FoV typically 90 degrees is good which is what this is set to
                                 float(w)/float(h), //Aspect Ratio, so Circles stay Circular
                                 0.01f, //Distance to the near plane, normally a small value like this
                                 100.0f); //Distance to the far plane, 
  return true;
}

glm::mat4 camera::GetProjection()
{
  return projection;
}

glm::mat4 camera::GetView()
{
  return view;
}

void camera::Update(unsigned int dt)
{
	eventType type;
	for (int i = 0; i < listener.getSize(); i++)
	{
		type = listener.getEvent(i);
		if (type.eventVer == SDL_MOUSEMOTION)
		{
			orbit -= type.x * 0.01f;
			if (distance < 8.0f)
				distance = 8.0f;
			else
				distance += type.y * 0.03f;
		}
	}

	if (parent != NULL) {
		//extract parent location
		lookAt = parent->GetModel() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}

	//calculate orbit and convert to position vector
	//orbit += dt * M_PI/1000;
	xPos = glm::sin(orbit);
	yPos = glm::cos(orbit);

	//translate it based on the parent position
	position = glm::vec3(xPos*distance, height, yPos*distance) + glm::vec3(lookAt);

	view = glm::lookAt( glm::vec3(position), //Eye Position
	                      glm::vec3(lookAt.x, lookAt.y, lookAt.z), //Focus point
	                      glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up
}

void camera::SetParent(Object *model)
{
	parent = model;
}
