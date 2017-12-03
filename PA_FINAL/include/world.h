
#ifndef WORLD_H_
#define WORLD_H_

#include "object.h"
#include "Light.h"
#include "physObject.h"
#include "kineObject.h"
#include "plane.h"
#include "imgui.h"

class World : public Object{
public:
	World();
	virtual ~World();
	void addLight(Light*);
	void loadWorld();
	virtual void Render();
	virtual void Update(unsigned int dt);
	Light* getLightData(int index);
	int getLightCount();
	GLuint getSkybox();
protected:
	virtual void keyboard(eventType);

private:
	GLuint skybox;

	btRigidBody *planeCollider;
	vector<Light*> lights;

	glm::vec3 cursor;
	KinematicObject *testphy;

	void initPhys();
};



#endif /* WORLD_H_ */
