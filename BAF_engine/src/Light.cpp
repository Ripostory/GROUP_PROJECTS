
#include "Light.h"

Light::Light()
{
	light = LightData(1, 1, LIGHT_POINT);
	light.pos = glm::vec3(0,0,0);
	loadModel("models/planet.obj");
}

Light::Light(int type)
{
	light = LightData(1, 1, type);
	light.pos = glm::vec3(0,0,0);
	loadModel("models/planet.obj");
}

Light::~Light()
{

}

void Light::Update(unsigned int dt)
{
	light.pos = glm::vec3(model * glm::vec4(0,0,0,1));
	light.radius = size;
}

LightData* Light::getLight()
{
	return &light;
}

void Light::setSize(float newSize)
{
	size = newSize;
	scale(newSize);
}

void Light::setColor(glm::vec3 newColor)
{
	light.color = newColor;
}
void Light::Render()
{
	  //no textures, only render model
	  glBindBuffer(GL_ARRAY_BUFFER, modelData.VB);
	  glEnableVertexAttribArray(0);
	  glEnableVertexAttribArray(1);

	  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,color));

	  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelData.IB);
	  glDrawElements(GL_TRIANGLES, modelData.size, GL_UNSIGNED_INT, 0);

	  glDisableVertexAttribArray(0);
	  glDisableVertexAttribArray(1);
}
