/*
 * moon.cpp
 *
 *  Created on: Sep 4, 2017
 *      Author: Ronn Quijada
 */

#include "moon.h"

Moon::Moon()
{
	angle = 0.0f;
	orbit = 0.0f;
	rotationSpeed = 1.0f;
	orbitSpeed = 1.0f;
	distance  = 1.0f;
	parent = NULL;
	size = 1.0f;
	multiplier = 1.0f;
	offset = 0.0f;
	tilt = 0.0f;
	isPlanet = true;
}

Moon::Moon (istream& is, Object* parent) {

	this -> parent = parent;

	float siz, speed;
	is >> siz >> speed;

	size = siz;
	rotationSpeed = speed;

	float angl, orbi, x, y, z;
	is >> angl >> orbi >> x >> y >> z;

	angle = angl;
	orbit = orbi;
	xPos = x;
	yPos = y;
	zPos = z;

	float dist, orbiSpeed, til, offs;
	is >> dist >> orbiSpeed >> til >> offs;

	distance = dist;
	orbitSpeed = orbiSpeed;
	tilt = til;
	offset = offs;

	bool isP, isG, isE, isR;
	is >> isP >> isG >> isE >> isR;

	isPlanet = isP;
	isGasGiant = isG;
	setEarth (isE);
	isaRing = isR;

	float horizonColX, horizonColY, horizonColZ;
	is >> horizonColX >> horizonColY >> horizonColZ;

	setHorizon(glm::vec3(horizonColX, horizonColY, horizonColZ));

	float atColX, atColY, atColZ;
	is >> atColX >> atColY >> atColZ;

	setAtmosphere(glm::vec3(atColX, atColY, atColZ));

	string mod;
	is >> mod;

	int num_textures;
	is >> num_textures;

	string* texts = new string [num_textures];

	for (int i = 0; i < num_textures; i++) {

		is >> texts[i];
	}

	setVisual (mod, texts [0], texts [1]);

	if (num_textures > 2) {

		for (int i = 2; i < num_textures; i++) {
			loadNewTexture (texts [i], i);
		}
	}

	delete[] texts;
	texts = NULL;


	return;
}

Moon::Moon(float rotSpeed, float orbSpeed, float dist, float siz, Object *parentModel)
{
	angle = 0.0f;
	orbit = rotSpeed * siz * dist;
	rotationSpeed = rotSpeed * ROTATION_MULT;
	orbitSpeed = orbSpeed * ORBIT_MULT;
	distance  = dist * DISTANCE_MULT;
	parent = parentModel;
	size = siz * SIZE_MULT;
	multiplier = 1.0f;
	offset = 0.0f;
	tilt = 0.0f;
	isPlanet = true;
}

Moon::Moon(float rotSpeed, float orbSpeed, float dist, float siz, float off, float til, Object *parentModel)
{
	angle = 0.0f;
	orbit = rotSpeed * siz * dist;
	rotationSpeed = rotSpeed * ROTATION_MULT;
	orbitSpeed = orbSpeed * ORBIT_MULT;
	distance  = dist * DISTANCE_MULT;
	parent = parentModel;
	size = siz * SIZE_MULT;
	multiplier = 1.0f;
	offset = off;
	tilt = til;
	isPlanet = true;
}

Moon::~Moon()
{

}

void Moon::Update(unsigned int dt)
{
	//get vec3 translation from parent mat4
	glm::vec4 parentPos = parent->GetModel() * glm::vec4(0.0, 0.0, 0.0, 1.0);

	//translate it based on the parent position
	model = glm::translate(glm::mat4(1.0f), (glm::vec3) parentPos);

	model = glm::rotate(model, (tilt), glm::vec3(0.0, 0.0, 1.0));
	//calculate orbit and convert to position vector
	orbit -= (multiplier * dt * M_PI/1000) /orbitSpeed;
	xPos = glm::sin(orbit);
	yPos = glm::cos(orbit);
	model = glm::translate(model, glm::vec3(xPos*distance, 0.0, yPos*distance));

	//original rotate code modified to take initial translated matrix
	angle += (multiplier *  dt * M_PI/1000) /rotationSpeed;
	model = glm::rotate(model, (angle), glm::vec3(0.0, 1.0, 0.0));

	//make moon smaller
	model = glm::scale(model, glm::vec3(size));
	model = glm::rotate(model, (offset), glm::vec3(1.0, 0.0, 0.0));
}

void Moon::setParent(Object *par)
{
	parent = par;
}

void Moon::setSize(float siz)
{
	size = siz;
}

std::ostream& operator<< (std::ostream& os, const Moon& moon) {

	os << moon.size << " " << moon.rotationSpeed << endl;
	os << moon.angle << " " << moon.orbit << " " << moon.xPos << " " << moon.yPos << " " << moon.zPos << endl;
	os << moon.distance << " " << moon.orbitSpeed << " " << " " << moon.tilt << " " << moon.offset << endl;

	os << moon.isPlanet << " " << moon.isGasGiant << " " << moon.earth << " " << moon.isaRing << endl;
	os << moon.horizonColor.x << " " << moon.horizonColor.y << " " << moon.horizonColor.z << endl;
	os << " " << moon.atmosphereColor.x << " " << moon.atmosphereColor.y << " " << moon.atmosphereColor.z << endl;

	os << moon.model_path << endl;
	os << moon.texture_paths.size () << endl;

	for (int i = 0; i < moon.texture_paths.size(); i++) {
		os << moon.texture_paths [i] << endl;
	}

	return os;
}




