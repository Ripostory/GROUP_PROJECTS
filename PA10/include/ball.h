
#ifndef BALL_H
#define BALL_H

#include "physObject.h"

class Ball : public PhysObject {

public:
	Ball (glm::vec3);
	void keyboard(eventType);
	void Update(unsigned int dt);
};



#endif
