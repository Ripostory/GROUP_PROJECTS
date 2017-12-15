

#ifndef INCLUDE_GUN_H_
#define INCLUDE_GUN_H_

#include "kineObject.h"
#include "Camera.h"
#include "Light.h"

class Gun : public KinematicObject {
public:
	Gun();
	Gun(camera*, Light*);
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

	Light *muzzle;
	bool activeBarrel;

	float barrel1z;
	float barrel2z;

	SDL_Keycode keyBind;

};

#endif /* INCLUDE_GUN_H_ */
