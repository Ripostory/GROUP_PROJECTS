
#ifndef WORLD_H_
#define WORLD_H_

#include "object.h"
#include "Light.h"
#include "physObject.h"
#include "imgui.h"

class World : public Object{
public:
	World();
	virtual ~World();
	void addLight(Light*);
	void loadWorld();
	virtual void Render();
	virtual void Update(unsigned int dt);
	static void setLightPointer(GLuint pos, GLuint rad, GLuint siz);
	Light* getLightData(int index);
	int getLightCount();
protected:
	virtual void keyboard(eventType);

private:
	static GLuint lightSize;
	static GLuint lightPosArray;
	static GLuint lightRadArray;
	GLuint skybox;

	btRigidBody *planeCollider;
	vector<Light*> lights;

	glm::vec3 cursor;

	void initPhys();
};



#endif /* WORLD_H_ */
