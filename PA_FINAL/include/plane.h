

#ifndef SRC_PLANE_H_
#define SRC_PLANE_H_

#include "kineObject.h"
#include "imgui.h"

#define THROTTLE 	1
#define TURN 		2
#define	PITCH 		3

class Plane : public KinematicObject {
public:
	Plane();
	~Plane();

	void moveTo(glm::vec3);
	void turnTo(glm::vec3);
	void Update(unsigned int dt);
	bool isTravelling();
private:
	float throttle;
	float turn;
	float tilt;
	float pitch;
	bool atDestination;
	bool turning;
	bool idling;
	glm::vec3 destination;
	glm::vec3 flyVector;
	glm::vec3 targetArea;

	std::queue<glm::vec3> events;

	void keyboard(eventType);
};



#endif /* SRC_PLANE_H_ */
