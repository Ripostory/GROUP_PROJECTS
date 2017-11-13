
#ifndef LIGHT_H_
#define LIGHT_H_

#include "object.h"

#define LIGHT_POINT	0

struct LightData {
	int type;
	float radius;
	float power;
	glm::vec3 pos;
	LightData(): radius(1), power(1), type(LIGHT_POINT) {}
	LightData(float r, float p, int t): radius(r), power(p), type(t) {}
};

class Light : public Object {
public:
	Light();
	~Light();
	void Render();
	void Update(unsigned int dt);
	LightData* getLight();
protected:
private:
	glm::vec3 color;
	LightData light;
};



#endif /* LIGHT_H_ */
