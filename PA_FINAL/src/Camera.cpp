
#include "Camera.h"

camera::camera()
{
    parent = NULL;
    world = NULL;
    index = -1;
    fov = 45;
    speed = 0.2;
    w = 0;
    h = 0;
    xAngle = 0;
    yAngle = 0;
    velocity = 0;

    position = glm::vec3(0.0, 32.0, 0.0);
    lookAt = glm::vec3(0.0);
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
  this->w = w;
  this->h = h;
  view = glm::lookAt( position, //Eye Position
                      lookAt, //Focus point
                      glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up

  projection = glm::perspective( 45.0f, //the FoV typically 90 degrees is good which is what this is set to
                                 float(w)/float(h), //Aspect Ratio, so Circles stay Circular
                                 0.01f, //Distance to the near plane, normally a small value like this
                                 10000.0f); //Distance to the far plane,
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

		//implement FPS style camera
		if (type.eventVer == SDL_MOUSEMOTION)
		{
			xAngle += (float) type.x * dt * 0.001 * speed;
			yAngle -= (float) type.y * dt * 0.0005 * speed;
		}
		else if (type.eventVer == SDL_MOUSEWHEEL)
		{
		}
		else if (type.eventVer == SDL_KEYDOWN)
		{
			if (type.key == SDLK_w)
			{
				velocity += dt * 0.05;
				if (velocity > MAX_VELOCITY)
					velocity = MAX_VELOCITY;
			}
			if (type.key == SDLK_s)
			{
				velocity -= dt * 0.05;
				if (velocity < -MAX_VELOCITY)
					velocity = -MAX_VELOCITY;
			}
		}
	}

	//update position
	position -= (position - lookAt) *  velocity;

	//update lookat
	lookAt = position;
	if (yAngle > 3.1415/2)
		yAngle = 3.1415/2;
	if (yAngle < -3.1415/2)
		yAngle = -3.1415/2;

	lookAt.x = lookAt.x +glm::cos(xAngle) * glm::cos(yAngle);
	lookAt.z = lookAt.z +glm::sin(xAngle) * glm::cos(yAngle);
	lookAt.y = lookAt.y +glm::sin(yAngle);

	ImGui::Text("look: <%f,%f,%f>", lookAt.x, lookAt.y, lookAt.z );
	ImGui::Text("angle: <%f,%f>", xAngle, yAngle);

	view = glm::lookAt( position, //Eye Position
	                    lookAt, //Focus point
	                    glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up
	if (velocity != 0)
	{
		if (velocity > 0)
			velocity -= 0.005 *dt;
		if (velocity < 0)
			velocity += 0.005 *dt;
		if (velocity >= -0.1 && velocity <= 0.1)
			velocity = 0;
	}
	ImGui::Text("velocity: %f", velocity);
}

void camera::SetWorld(Object *model)
{
	world = model;
}

void camera::SetParent(Object *model)
{
	parent = model;
}

glm::vec3 camera::GetPosition()
{
	return position;
}

glm::vec3 camera::getLookat() {
	return lookAt - position;
}
