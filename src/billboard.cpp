/*
 * billboard.cpp
 *
 *  Created on: Oct 28, 2017
 *      Author: nog
 */

#include "billboard.h"

float Billboard::quadData[] = {
	    -1.0f,  1.0f, 0.0f, 1.0f, // Top-left
	     1.0f,  1.0f, 1.0f, 1.0f, // Top-right
	     1.0f, -1.0f, 1.0f, 0.0f, // Bottom-right

	     1.0f, -1.0f, 1.0f, 0.0f, // Bottom-right
	    -1.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
	    -1.0f,  1.0f, 0.0f, 1.0f, // Top-left
};
GLuint Billboard::quad;

Billboard::Billboard()
{
	size = 1.0f;
	position = glm::vec3(0.0);
	image = 0;
}

Billboard::~Billboard()
{
	size = 0.0f;
	position = glm::vec3(0.0);
}

void Billboard::Update(unsigned int dt)
{
	//update billboard matrix
	billboard = glm::translate(glm::mat4(1.0f),position);
	billboard = glm::scale(billboard, glm::vec3(size));
}

void Billboard::Render()
{
	//render billboard to screen
	//NOTE: may not be used in particle systems
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_CULL_FACE);
	glBindBuffer(GL_ARRAY_BUFFER, quad);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	//draw quad onto the screen
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
	                         4*sizeof(float), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
	                         4*sizeof(float), (void*)(2*sizeof(float)));

	//pass in output texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, image);
	glDrawArrays(GL_TRIANGLES, 0 ,6);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void Billboard::init()
{
	//init quad
	glGenBuffers(1, &quad);
	glBindBuffer(GL_ARRAY_BUFFER, quad);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadData), quadData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Billboard::end()
{
	//delete quad
	glDeleteBuffers(1, &quad);
}

void Billboard::translate(glm::vec3 move)
{
	position = move;
}

void Billboard::scale(float scale)
{
	size = scale;
}

void Billboard::setImage(GLuint newImage)
{
	image = newImage;
}

void Billboard::setImage(std::string filename)
{
	loader load;
	image = load.loadTexture(filename).texture;
}

glm::vec3 Billboard::getPosition()
{
	return position;
}

float Billboard::getSize()
{
	return size;
}

glm::mat4 Billboard::getMatrix()
{
	return billboard;
}
