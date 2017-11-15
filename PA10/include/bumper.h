#ifndef INCLUDE_BUMPER_H
#define INCLUDE_BUMPER_H

#include "physObject.h"
#include "ball.h"
#include "gui.h"

#define FORCE_MULT 1000

class Bumper : public PhysObject {
public:
	Bumper();
	Bumper(glm::vec3 position);
	~Bumper();
protected:
	void OnCollisionDetected (PhysObject*);

private:
	void keyboard(eventType);
	void Update(unsigned int dt);
};



#endif /* INCLUDE_PADDLE_H_ */
