
#include "Light.h"

Light::Light()
{
	light = LightData(5, 1, LIGHT_POINT);
}

Light::~Light()
{

}

void Light::Update(unsigned int dt)
{
	light.pos = glm::vec3(glm::vec4(0,0,0,1) * model);
}

void Light::Render()
{
	//ignore rendering
}

