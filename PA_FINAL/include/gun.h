

#ifndef INCLUDE_GUN_H_
#define INCLUDE_GUN_H_

#include "kineObject.h"
#include "Camera.h"

class Gun : public KinematicObject {
public:
	Gun();
	Gun(camera*);
	~Gun();
	virtual void Update(unsigned int dt);
	void spawnTracer();
private:
	virtual void keyboard(eventType);
	camera *Cam;
};

#endif /* INCLUDE_GUN_H_ */
