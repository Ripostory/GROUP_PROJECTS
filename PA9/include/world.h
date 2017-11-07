
#ifndef WORLD_H_
#define WORLD_H_

#include "object.h"
#include "Light.h"
#include "physObject.h"

class World : public Object{
public:
	World();
	virtual ~World();
	void addLight(Light*);
	virtual void Render();
	virtual void Update(unsigned int dt);
	static void setLightPointer(GLuint pos, GLuint rad, GLuint siz, GLuint rot, GLuint types);
protected:
	virtual void keyboard(eventType);

private:
	static GLuint lightSize;
	static GLuint lightPosArray;
	static GLuint lightRadArray;
	static GLuint lightTypeArray; //Type to differentiate spot from point
	static GLuint lightRotArray; //Rotation for spotlight

	btRigidBody *planeCollider;
	vector<Light*> lights;

	Light* spot;
	Object* lastBall;


	glm::vec3 *pos;
	glm::vec3 *rad;
	glm::vec3 *rot;
	int* types;

	void initPhys();
	void rebuildDataArray();
};



#endif /* WORLD_H_ */
