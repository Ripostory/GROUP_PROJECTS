
#ifndef WORLD_H_
#define WORLD_H_

#include "object.h"
#include "physObject.h"

class World : public Object{
public:
	World();
	virtual ~World();
private:
	btRigidBody *planeCollider;

	void initPhys();
};



#endif /* WORLD_H_ */
