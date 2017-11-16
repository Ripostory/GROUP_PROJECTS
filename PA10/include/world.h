
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
	void removeLight (Light*);
	virtual void Render();
	virtual void Update(unsigned int dt);
	static void setLightPointer(GLuint pos, GLuint rad, GLuint siz, GLuint col);

	static World* GetInstance ();
protected:
	virtual void keyboard(eventType);

private:
	static GLuint lightSize;
	static GLuint lightPosArray;
	static GLuint lightRadArray;
	static GLuint lightColArray;

	btRigidBody *planeCollider;
	vector<Light*> lights;

	glm::vec3 *pos;
	glm::vec3 *color;

	void initPhys();
	void rebuildDataArray(); 

	static World* m_instance;
};



#endif /* WORLD_H_ */
