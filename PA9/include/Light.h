
#ifndef LIGHT_H_
#define LIGHT_H_

#include "object.h"


//I prefer enums because they make the code easier to understand
enum LightType { Light_Point = 0, Light_Spot = 1 };

struct LightData {
	int type;
	float radius;
	float power;
	glm::vec3 pos;
	glm::vec3 rotation; //Direction only affects spotlight
	LightData(): radius(1), power(3), type(Light_Point) {}
	LightData(float r, float p, LightType t): radius(r), power(p), type(t) {}
};

class Light : public Object {

public:
	Light(); //Default constructor builds point light
	Light (float r, float p, LightType t);
	~Light();
	void Render();
	void Rotate (float x, float y, float z);
	void PointTowards (glm::vec3 lookAt); //Does the math for a rotation for you!
	void Update(unsigned int dt);
	LightData* getLight();
protected:
private:
	glm::vec3 color;
	LightData light;
};



#endif /* LIGHT_H_ */
