
#include "plane.h"

Plane::Plane()
{
	throttle = 0;
	turn = 1;
	pitch = 0;
	atDestination = false;
	destination = glm::vec3(0);
	position = glm::vec3(0, 10, 0);
	flyVector = glm::vec3(1,0,0);

	loadModel("Plane.obj");
	loadTexture("n_plane.png");
	loadTexture("n_plane.png", 2);
	loadNormal("cleanNormal.png");

	animator.animateFloat(&throttle, 90, 5, linear, 1);
	animator.animateFloat(&throttle, 60, 5, linear, 1);
	animator.animateFloat(&turn, 2, 5, linear, 1);
	animator.animateFloat(&turn, -1, 5, linear, 1);
}

Plane::~Plane()
{

}

void Plane::Update(unsigned int dt)
{

	//TODO fix
	//update flyVector
	glm::mat4 rotationmat = glm::rotate(turn*((float)dt/1000.0f), glm::vec3(0,1,0));
	flyVector = glm::vec3(rotationmat * glm::vec4(flyVector, 1.0));

	translateBy(-flyVector*((float)dt/1000.0f)*throttle);
	//rotate towards flyVector
	rotateTo(glm::vec3(-flyVector.x, flyVector.y, flyVector.z), glm::vec3(0,1,0));
	model *= glm::rotate(-turn/(3.14f), glm::vec3(0,0,1));
	KinematicObject::Update(dt);

	ImGui::Text("Plane vector: <%.02f, %.02f, %.02f>", flyVector.x, flyVector.y, flyVector.z);
	ImGui::Text("Plane location: <%.02f, %.02f, %.02f>", position.x, position.y, position.z);
	ImGui::Text("Plane Throttle: <%.02f>", throttle);
}

void Plane::keyboard(eventType)
{

}

void Plane::moveTo(glm::vec3 newDestination)
{
	if (newDestination != destination)
	{
		atDestination = false;
		destination = newDestination;
	}
}

//TODO implement
bool Plane::isTravelling()
{
	return atDestination;
}
