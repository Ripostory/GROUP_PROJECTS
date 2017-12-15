
#include "ocean.h"

Ocean::Ocean()
{
	offset = 0.0f;
	//TODO add correct textures
	loadModel("ocean.obj");
	loadTexture("a_earth.jpg");
	loadTexture("s_earth.jpg", 2);
	loadNormal("n_earth.jpg");
}

Ocean::~Ocean()
{

}

void Ocean::Update(unsigned int dt)
{
	//update waves
	scale(5.0f);
	offset += (float)dt/1000.0f;
}

void Ocean::Render()
{
	//pass in texture offset
	glUniform1f(3, offset);
	Object::Render();
}


float Ocean::getOffset()
{
	return offset;
}
