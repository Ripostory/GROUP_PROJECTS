

#ifndef INCLUDE_GUN_H_
#define INCLUDE_GUN_H_

#include "kineObject.h"
#include "Camera.h"
#include "Light.h"
#include "sound.h"

class Gun : public KinematicObject {
public:
	Gun();
	Gun(camera*, Light*);
	~Gun();
	virtual void Update(unsigned int dt);
	virtual void Render();
	void setEnabled(bool);
	void kill();
	void spawnTracer();

	Sound* sample;

private:
	virtual void keyboard(eventType);
	void initBarrel(Object*);
	camera *Cam;
	glm::vec3 lastLookat;

	Object *barrel1;
	Object *barrel2;

	Light *muzzle;
	bool activeBarrel;
	bool firing;
	bool enabled;

	float barrel1z;
	float barrel2z;
	float recoil;

	SDL_Keycode keyBind;

};

#endif /* INCLUDE_GUN_H_ */
