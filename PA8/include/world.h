
#ifndef WORLD_H_
#define WORLD_H_

#include "object.h"
#include "physObject.h"

class World : public Object{
public:
	World();
	virtual ~World();
protected:
	virtual void keyboard(eventType);

private:
	btRigidBody *planeCollider;
	PhysObject* paddle;

	void initPhys();
};



#endif /* WORLD_H_ */
