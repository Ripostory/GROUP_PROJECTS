
#ifndef LIGHT_H_
#define LIGHT_H_

#include "object.h"

#define LIGHT_AMB	-1
#define LIGHT_POINT	0
#define LIGHT_DIR	1

struct LightData {
	int type;
	float radius;
	float power;
	glm::vec3 color;
	glm::vec3 pos;
	LightData(): radius(1), power(1), type(LIGHT_POINT) {}
	LightData(float r, float p, int t): radius(r), power(p), type(t) {}
};

class Light : public Object {
public:
	Light();
	Light(int type);
	~Light();
	void Update(unsigned int dt);
	void Render();
	void setSize(float);
	void setColor(glm::vec3);
	void setParent(Object*);
	LightData* getLight();
protected:
private:
	LightData light;
	Object *parent;
};



#endif /* LIGHT_H_ */
