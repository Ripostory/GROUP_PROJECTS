
#include "plane.h"

Plane::Plane()
{
	throttle = 50;
	turn = 0;
	pitch = 60;
	tilt = 0;
	atDestination = false;
	destination = glm::vec3(0);
	flyVector = glm::vec3(1,0,0);
	turning = false;
	idling = false;

	loadModel("Plane.obj");
	loadTexture("n_plane.png");
	loadTexture("n_plane.png", 2);
	loadNormal("cleanNormal.png");
	setCollisionMesh(PHYS_SPHERE, 10);

	translate(glm::vec3(0, pitch, 0));
	events.push(glm::vec3(254, pitch , 162));
	events.push(glm::vec3(0, pitch, 0));
	events.push(glm::vec3(-168, pitch, -99));
	events.push(glm::vec3(221, pitch, -76));

	Billboard *test = new Billboard();
	test->setImage("a_earth.jpg");
	test->translate(glm::vec3(0,20,0));
	test->scale(1.0);
	addUI(test);
}

Plane::~Plane()
{

}

void Plane::Update(unsigned int dt)
{
	if (!events.empty())
	{
		if (!animator.isPending() && idling)
		{
			targetArea = events.front();
			events.pop();
		}
	}
	moveTo(targetArea);
	//TODO figure out translation height
	//update flyVector
	glm::mat4 rotationmat = glm::rotate(turn*((float)dt/1000.0f), glm::vec3(0,1,0));
	flyVector = glm::vec3(rotationmat * glm::vec4(flyVector, 1.0));

	translateBy(-flyVector*((float)dt/1000.0f)*throttle);
	//rotate towards flyVector
	rotateTo(glm::vec3(-flyVector.x, flyVector.y, flyVector.z), glm::vec3(0,1,0));

	//modify tilt
	if (glm::abs(tilt - turn) > 0.01)
	{
		if (tilt > turn)
			tilt -= glm::abs(turn-tilt)*0.05;
		else if (tilt < turn)
			tilt += glm::abs(turn-tilt)*0.05;
	}

	model *= glm::rotate(-tilt/(3.14f), glm::vec3(0,0,1));
	KinematicObject::Update(dt);

	ImGui::Text("Plane vector: <%.02f, %.02f, %.02f>", flyVector.x, flyVector.y, flyVector.z);
	ImGui::Text("Plane Throttle: <%.02f>", throttle);
	ImGui::Text("Plane tilt: <%.02f>", tilt);
	ImGui::Text("Plane turn: <%.02f>", turn);
	ImGui::Text("Queued animations: %i", animator.getAnimationCount());
}

void Plane::keyboard(eventType event)
{
	if (event.eventVer == SDL_KEYDOWN)
	{
		if (event.key == SDLK_h)
		{
			targetArea = glm::vec3(50,0,60);
		}
		if (event.key == SDLK_g)
		{
			targetArea = glm::vec3(-200,0,30);
		}
	}
}

void Plane::moveTo(glm::vec3 newDestination)
{
	glm::vec3 position = glm::vec3(model[3][0],model[3][1],model[3][2]);
	if (newDestination != destination)
	{
		atDestination = false;
		idling = false;
		destination = newDestination;

		//queue actions
		turnTo(glm::normalize(position - destination));
	}
	else
	{
		//check if near destination
		if (!atDestination)
		{
			if (glm::distance(destination, position) <= 100)
			{
				atDestination = true;
			}
			else
			{
				if (!animator.isPending())
					//plane may be lost, reroute
					turnTo(glm::normalize(position - destination));
			}

		}

		//on reaching destination
		if (atDestination && !idling)
		{
			if (turn < 1 && turn > -1)
			{
				animator.animateFloat(&turn,1,2,linear,TURN);
				animator.animateFloat(&throttle,50,3,linear,THROTTLE);
				//delay
				animator.animateFloat(&turn,1,5,none,TURN);
				atDestination = true;
				idling = true;
			}
		}
	}
}

void Plane::turnTo(glm::vec3 direction)
{
	//turn towards a vector

	animator.interrupt(TURN);
	//get theta
	float aDotB = glm::dot(glm::normalize(direction), glm::normalize(flyVector));
	float angle = glm::acos(aDotB)*180.0f/3.1415926;

	if (angle > 3)
	{
		//figure out which way to turn
		float time;
		float turnDir;
		if (glm::cross(direction, flyVector).y > 0)
		{
			//7.4 seconds approx = 180 degrees
			turnDir = -1;
			time = 3.7*(angle/180);
		}
		else
		{
			turnDir = 1;
			time = 3.7*(angle/180);
		}

		//animate to vector
		animator.animateFloat(&turn,turnDir,time*(1.0f/6.0f),linear,TURN);
		animator.animateFloat(&turn,turnDir,time*(4.0f/6.0f),none,TURN);
		animator.animateFloat(&turn,0,time*(1.0f/6.0f),linear,TURN);
	}
}

bool Plane::isTravelling()
{
	return atDestination;
}



void Plane::OnCollisionDetected (PhysObject* hit) {

	cout << "Kinematic object hit another object" << endl;
}
void Plane::OnRaycastHit () {

	cout << "Plane at " << this << " hit by ray" << endl;
}
