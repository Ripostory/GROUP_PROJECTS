
#include "plane.h"

Plane::Plane(Light *effect, float spnHeight, float spnSpeed, float spnHP)
{
	baseSpeed = spnSpeed;

	throttle = baseSpeed;
	turn = 0;
	pitch = 0;
	tilt = 0;
	height = spnHeight;
	atDestination = true;
	destination = glm::vec3(900, height, 0);
	flyVector = glm::vec3(1,0,0);
	turning = false;
	idling = true;
	hp = spnHP;
	crashing = false;
	startedCrashing = false;
	explosion = false;
	effectLight = effect;

	loadModel("Plane.obj");
	loadTexture("n_plane.png");
	loadTexture("n_plane.png", 2);
	loadNormal("cleanNormal.png");
	setCollisionMesh(PHYS_SPHERE, 10);

	effectLight->translate(glm::vec3(0, -3, 0));
	effectLight->setSize(50);
	effectLight->setColor(glm::vec3(0,0,0));
	effectLight->setParent(this);

	translate(glm::vec3(900, height, 0));
	generateFlyPath();

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
	KinematicObject::Update(dt);

	//update flyVector
	glm::mat4 rotationmat = glm::rotate(pitch*((float)dt/1000.0f), glm::vec3(1,0,0));
	rotationmat *= glm::rotate(turn*((float)dt/1000.0f), glm::vec3(0,1,0));

	flyVector = glm::vec3(rotationmat * glm::vec4(flyVector, 1.0));

	translateBy(-flyVector*((float)dt/1000.0f)*throttle);
	//rotate towards flyVector
	rotateTo(flyVector, glm::vec3(0,1,0));

	if (crashing)
	{
		if (!startedCrashing)
		{
			turn = 0;
			animator.interrupt(TURN);
			animator.interrupt(THROTTLE);
			animator.interrupt(PITCH);
			animator.animateFloat(&tilt, 500, 10, easein, TURN);
			animator.animateFloat(&pitch, -1, 5, linear, PITCH);
			animator.animateFloat(&throttle, 100, 5, linear, THROTTLE);
			startedCrashing = true;
		}
		glm::vec3 position = getPosition();
		if (position.y > -2)
			translateBy(glm::vec3(0, pitch,0));
		else
		{
			//TODO add completed crash stuff here
			turn = 0;
			pitch = 0;
			throttle = 0;
			animator.interrupt(TURN);
			animator.interrupt(THROTTLE);
			animator.interrupt(PITCH);

			//explode if we hit the water
			if (!explosion)
			{
				explode();
			}

			//reset to default location once the explosion ends
			if (!effectLight->animator.isPending())
				effectLight->translate(glm::vec3(0,-1000,0));
		}
	}
	else
		regularUpdate(dt);

	model *= glm::rotate(-tilt/(3.14f), glm::vec3(0,0,1));

	//clean gibs
	if (children.size() != 0)
	{
		if (!children.front()->animator.isPending(999))
		{
			children.erase(children.begin());
		}
	}

	//ImGui::Text("Plane vector: <%.02f, %.02f, %.02f>", flyVector.x, flyVector.y, flyVector.z);
	//ImGui::Text("Plane Throttle: <%.02f>", throttle);
	//ImGui::Text("Plane tilt: <%.02f>", tilt);
	//ImGui::Text("Plane turn: <%.02f>", turn);
	//ImGui::Text("Queued animations: %i", animator.getAnimationCount());
}

void Plane::regularUpdate(unsigned int dt)
{
	//keep moving to target area
	if (!events.empty())
	{
		if (!animator.isPending() && idling)
		{
			targetArea = events.front();
			events.pop();
		}
	}
	moveTo(targetArea);

	//modify tilt
	if (glm::abs(tilt - turn) > 0.01)
	{
		if (tilt > turn)
			tilt -= glm::abs(turn-tilt)*0.05;
		else if (tilt < turn)
			tilt += glm::abs(turn-tilt)*0.05;
	}
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
	glm::vec3 position = getPosition();
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
				animator.animateFloat(&throttle,baseSpeed,3,linear,THROTTLE);
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

void Plane::OnCollisionDetected (PhysObject* hit)
{
	//we hit a ship, explode
	if (crashing && !explosion)
		explode();
}

void Plane::OnRaycastHit ()
{
	if (!crashing)
	{
		hp -= 5.0;
		if (hp < 0.0)
			crashing = true;
	}
	else
	{

	}

	//animate plane light
	float damage = (hp-100)/100;
	effectLight->animator.interrupt(10);
	effectLight->animator.interrupt(11);
	effectLight->animator.interrupt(12);
	effectLight->animator.interrupt(20);
	effectLight->translate(glm::vec3(-10, 5, 10));
	effectLight->lerpTo(glm::vec3(10,-5,-10), 0.2);
	effectLight->setColor(glm::vec3(5*7,3*7,1*7));
	effectLight->changeColor(glm::vec3(1*damage,0.2*damage,0),0.5, linear);
}

void Plane::generateFlyPath()
{
	for (int i = 0; i < (rand() % 3 + 4); i++) {
		float xrand = rand() % 200 + 150;
		float zrand = rand() % 400 - 200;
		events.push(glm::vec3(xrand, height, zrand));
	}

	//final pass
	events.push(glm::vec3(600, height, 0));
	events.push(glm::vec3(-400, height, 0));
}

void Plane::explode()
{

	//animate effect light
	glm::vec3 finalPos = getPosition();
	finalPos.y += 40;
	effectLight->animator.interrupt(10);
	effectLight->animator.interrupt(11);
	effectLight->animator.interrupt(12);
	effectLight->animator.interrupt(20);
	effectLight->setSize(150.0f);
	effectLight->setParent(NULL);
	effectLight->translate(finalPos);
	effectLight->lerpTo(glm::vec3(finalPos.x+2,finalPos.y,finalPos.z-2), 0.1);
	effectLight->lerpTo(glm::vec3(finalPos.x-4,finalPos.y,finalPos.z+2), 0.3);
	effectLight->lerpTo(glm::vec3(finalPos.x+1,finalPos.y-20,finalPos.z-1), 0.4);
	effectLight->changeColor(glm::vec3(100,90,80), 0.1, easeinout);
	effectLight->changeColor(glm::vec3(12*7,10*7,4*7), 0.4, easeinout);
	effectLight->changeColor(glm::vec3(5,2,0), 0.6, easeinout);
	effectLight->changeColor(glm::vec3(0),1, easeinout);

	//add gibs
	PhysObject *gib;
	float randforcex;
	float randforcey;
	float randforcez;
	glm::vec3 force;

	for (int i = 0; i < 3; i++) {
		randforcex = (rand() % 200 - 100) - flyVector.x*100;
		randforcez = (rand() % 200 - 100) - flyVector.z*100;
		randforcey = rand() % 40 - 20;
		force = glm::vec3(randforcex,randforcey+flyVector.y*750, randforcez);

		gib = new PhysObject();
		gib->loadModel("Plane.obj");
		gib->loadNormal("cleanNormal.png");
		gib->setCollisionMesh(PHYS_SPHERE, 3);
		gib->setProperties((rand()%5)/2 + 0.5,1,0.4);
		gib->translate(getPosition());
		gib->initPhysics();
		gib->applyForce(force);
		gib->setDamping(0.3,0.7);
		gib->animator.timer(10,999);
		addChild(gib);
	}

	//hide plane
	setRenderable(false);

	explosion = true;
}

bool Plane::isDead()
{
	return crashing;
}

bool Plane::isDeletable()
{
	return (explosion && children.size() == 0);
}

bool Plane::isEscaped()
{
	return (events.size() == 0 && idling);
}


/*=============
 * START OF SQUADRON CODE
 ==============*/

Squadron::Squadron(vector<Light*> source, float difficulty)
{
	setRenderable(false);
	defeated = false;
	planeEscape = false;
	planeCount = rand() % 4 + 3;
	Plane *plane;
	float hpScale;
	float speedScale;
	float height;

	//scale speed and hp based on difficulty
	hpScale = 20 + difficulty*10;
	speedScale = 50 + difficulty*10;
	if (speedScale > 100)
		speedScale = 100;

	//set base height
	height = 100;

	//get list of lights for planes
	vector<Light*>::iterator it;
	it = source.begin();
	it += 3;

	//spawn planes
	for (int i = 0; i < planeCount; i++, height += 20)
	{
		plane = new Plane(*it, height, speedScale, hpScale);
		addChild(plane);
		planeList.push_back(plane);
		it++;
	}
}

Squadron::~Squadron()
{

}

void Squadron::Update(unsigned int dt)
{
	Object::Update(dt);

	int destroyed = 0;
	int endAnimation = 0;
	Plane *plane;
	//check if all planes have been defeated
	for (int i = 0; i < planeCount; i++)
	{
		plane = planeList[i];
		if (plane->isDead())
			destroyed++;

		if (plane->isDeletable())
			endAnimation++;

		if (plane->isEscaped())
			planeEscape = true;
	}

	ImGui::Text("Planes destroyed: %i", destroyed);

	if (destroyed == planeCount)
	{
		//all planes destroyed
		ImGui::Text("All planes destroyed!");
	}

	if (endAnimation == planeCount)
	{
		defeated = true;
		ImGui::Text("Plane squadron safe to delete");
	}

	if (planeEscape)
	{
		ImGui::Text("Plane ESCAPED");
	}
}

bool Squadron::isDefeated()
{
	return defeated;
}

bool Squadron::isPlaneEscape()
{
	return planeEscape;
}





