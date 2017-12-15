

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
	virtual void Render();
	void spawnTracer();
private:
	virtual void keyboard(eventType);
	void initBarrel(Object*);
	camera *Cam;
	glm::vec3 lastLookat;

	Object *barrel1;
	Object *barrel2;
};

#endif /* INCLUDE_GUN_H_ */
