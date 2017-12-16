

#ifndef SRC_PLANE_H_
#define SRC_PLANE_H_

#include "kineObject.h"
#include "imgui.h"
#include "Light.h"

#define THROTTLE 	1
#define TURN 		2
#define	PITCH 		3

class Plane : public KinematicObject {
public:
	Plane(Light*);
	~Plane();

	void moveTo(glm::vec3);
	void turnTo(glm::vec3);
	void Update(unsigned int dt);
	bool isTravelling();

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

	float hp;

	std::queue<glm::vec3> events;

	void keyboard(eventType);
};



#endif /* SRC_PLANE_H_ */
