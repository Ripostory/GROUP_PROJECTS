/*
 * billboard.h
 *
 *  Created on: Oct 28, 2017
 *      Author: nog
 */

#ifndef INCLUDE_BILLBOARD_H_
#define INCLUDE_BILLBOARD_H_

#include "graphics_headers.h"
#include "string.h"
#include "loader.h"

class Billboard {
public:
	Billboard();
	virtual ~Billboard();

	static void init();
	static void end();

	void translate(glm::vec3);
	void scale(float);
	void setImage(GLuint);
	void setImage(std::string);

	glm::vec3 getPosition();
	glm::mat4 getMatrix();
	float getSize();

	virtual void Update(unsigned int dt);
	virtual void Render();

private:
	static GLuint quad;
	static float quadData[24];
	glm::vec3 position;
	glm::mat4 billboard;
	float size;
	GLuint image;

};



#endif /* INCLUDE_BILLBOARD_H_ */
