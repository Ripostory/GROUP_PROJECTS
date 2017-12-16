

#ifndef SRC_PLANE_H_
#define SRC_PLANE_H_

#include "kineObject.h"
#include "imgui.h"
#include "Light.h"

#define THROTTLE 	1
#define TURN 		2
#define	PITCH 		3
#define MAX_PLANES 	15

class Plane : public KinematicObject {
public:
	Plane(Light*, float height, float speed, float hp);
	~Plane();

	void moveTo(glm::vec3);
	void turnTo(glm::vec3);
	void Update(unsigned int dt);
	bool isTravelling();
	bool isEscaped();
	bool isDead();
	bool isDeletable();

	void OnCollisionDetected (PhysObject* hit);
	void OnRaycastHit ();

private:
	float throttle;
	float turn;
	float tilt;
	float pitch;
	float height;
	float baseSpeed;
	bool atDestination;
	bool turning;
	bool idling;
	bool crashing;
	bool startedCrashing;
	bool explosion;

	Light *effectLight;
	glm::vec3 destination;
	glm::vec3 flyVector;
	glm::vec3 targetArea;

	void regularUpdate(unsigned int dt);
	void generateFlyPath();
	void explode();

	float hp;

	std::queue<glm::vec3> events;

	void keyboard(eventType);
};

class Squadron : public Object {
public:
	Squadron(vector<Light*>, float difficulty);
	~Squadron();

	void Update(unsigned int dt);
	bool isDefeated();
	bool isPlaneEscape();

private:
	bool defeated;
	bool planeEscape;
	int planeCount;
	vector<Plane*> planeList;
};

#endif /* SRC_PLANE_H_ */
