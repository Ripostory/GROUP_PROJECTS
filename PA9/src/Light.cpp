
#include "Light.h"

Light::Light()
{
	light = LightData(5, 1, Light_Point);
}

Light::	Light (float r, float p, LightType t) {

		light = LightData (r, p, t);
}

Light::~Light()
{

}

void Light::Update(unsigned int dt)
{
	light.pos = glm::vec3(model * glm::vec4(0,0,0,1));
}

void Light::Rotate (float x, float y, float z) {

	light.rotation = glm::vec3 (x, y, z);
}

void Light::PointTowards (glm::vec3 lookAtPoint) {

}

void Light::Render()
{
	//ignore rendering
}

LightData* Light::getLight()
{
	return &light;
}
