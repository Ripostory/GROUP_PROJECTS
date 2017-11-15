
#ifndef WORLD_H_
#define WORLD_H_

#include "object.h"
#include "Light.h"
#include "physObject.h"
#include "paddle.h"
#include "plunger.h"
#include "bumper.h"
#include "gui.h"
#include "ball.h"

class World : public Object{
public:
	World();
	virtual ~World();
	void addLight(Light*);
	virtual void Render();
	virtual void Update(unsigned int dt);
	static void setLightPointer(GLuint pos, GLuint rad, GLuint siz);

	static World* instance;
protected:
	virtual void keyboard(eventType);

private:
	static GLuint lightSize;
	static GLuint lightPosArray;
	static GLuint lightRadArray;

	btRigidBody *planeCollider;
	vector<Light*> lights;

	glm::vec3 *pos;

	void initPhys();
	void rebuildDataArray(); 
};



#endif /* WORLD_H_ */
