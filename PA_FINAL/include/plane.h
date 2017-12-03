

#ifndef SRC_PLANE_H_
#define SRC_PLANE_H_

#include "kineObject.h"
#include "imgui.h"

class Plane : public KinematicObject {
public:
	Plane();
	~Plane();

	void moveTo(glm::vec3);
	void Update(unsigned int dt);
	bool isTravelling();
private:
	float throttle;
	float turn;
	float pitch;
	bool atDestination;
	glm::vec3 destination;
	glm::vec3 flyVector;
	glm::vec3 position;

	void keyboard(eventType);
};



#endif /* SRC_PLANE_H_ */
