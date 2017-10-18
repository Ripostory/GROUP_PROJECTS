
#ifndef WORLD_H_
#define WORLD_H_

#include "object.h"
#include "Light.h"
#include "physObject.h"

class World : public Object{
public:
	World();
	virtual ~World();
	void addLight(Light);
	virtual void Render();
	virtual void Update(unsigned int dt);
protected:
	virtual void keyboard(eventType);

private:
	btRigidBody *planeCollider;
	vector<Light> lights;

	void initPhys();
};



#endif /* WORLD_H_ */
