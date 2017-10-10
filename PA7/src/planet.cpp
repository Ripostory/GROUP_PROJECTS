/*
 * planet.cpp
 *
 *  Created on: Sep 4, 2017
 *      Author: Ronn Quijada
 */

#include "planet.h"

Planet::Planet()
{
	  //updated file loading, default to cube object
	  size = 5.0f;
	  angle = 0.0f;
	  orbit = 0.0f;

	  rotationSpeed = 0.3f;
	  orbitSpeed = 0.2f;
	  distance = 20.0f;
	  multiplier = 1.0f;
	  tilt = 0.0f;
	  offset = 0.0f;
	  isPlanet = true;
}

//Constructor takes an input stream to build planet w/moons
Planet::Planet (istream& is) {

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

	int num_moons;
	is >> num_moons;

	for (int i = 0; i < num_moons; i++) {

		Object* moon;
	
		moon = new Moon (is, this);

		addChild (moon);
	}

	return;
}

Planet::Planet(float rotSpeed, float orbSpeed, float dist, float siz)
{
	  //seed starting angle
	  angle = 0.0f;
	  orbit = siz * dist;

	  //modify with multipliers
	  rotationSpeed = rotSpeed * ROTATION_MULT;
	  orbitSpeed = orbSpeed * ORBIT_MULT;
	  distance = dist * DISTANCE_MULT;
	  size = siz * SIZE_MULT;
	  multiplier = 1.0f;
	  tilt = 0.0f;
	  offset = 0.0f;
	  isPlanet = true;
}

Planet::Planet(float rotSpeed, float orbSpeed, float dist, float siz, float til, float offs)
{
	  //seed starting angle
	  angle = 0.0f;
	  orbit = siz * dist;

	  //modify with multipliers
	  rotationSpeed = rotSpeed * ROTATION_MULT;
	  orbitSpeed = orbSpeed * ORBIT_MULT;
	  distance = dist * DISTANCE_MULT;
	  size = siz * SIZE_MULT;
	  multiplier = 1.0f;
	  tilt = til;
	  offset = offs;
	  isPlanet = true;
}

Planet::~Planet()
{
}

void Planet::Update(unsigned int dt)
{
	  //calculate orbit and convert to position matrix
	  orbit -= (multiplier * dt * M_PI/1000) /orbitSpeed;
	  xPos = glm::sin(orbit);
	  yPos = glm::cos(orbit);
	  zPos = xPos;
	  model = glm::translate(glm::mat4(1.0f), glm::vec3(xPos*distance, zPos*offset, yPos*distance));

	  //add planet tilt
	  model = glm::rotate(model, (tilt), glm::vec3(0.0, 0.0, 1.0));
	  //original rotate code modified to take initial translated matrix
	  angle += (multiplier *  dt * M_PI/1000) /rotationSpeed;
	  model = glm::rotate(model, (angle), glm::vec3(0.0, 1.0, 0.0));

	  //scale model based on size;
	  model = glm::scale(model, glm::vec3(size));

	  //update children
	  for (int i = 0; i < children.size(); i++)
	  {
		  children[i]->setMultiplier(multiplier);
		  children[i]->Update(dt);
	  }
}

void Planet::setSize(float siz)
{
	size = siz;
}

std::ostream& operator<< (std::ostream& os, const Planet& planet) {

	os << planet.size << " " << planet.rotationSpeed << endl;
	os << planet.angle << " " << planet.orbit << " " << planet.xPos << " " << planet.yPos << " " << planet.zPos << endl;
	os << planet.distance << " " << planet.orbitSpeed << " " << planet.tilt << " " << planet.offset << endl;

	os << planet.isPlanet << " " << planet.isGasGiant << " " << planet.earth << " " << planet.isaRing << endl;
	os << planet.horizonColor.x << " " << planet.horizonColor.y << " " << planet.horizonColor.z << endl;
	os << " " << planet.atmosphereColor.x << " " << planet.atmosphereColor.y << " " << planet.atmosphereColor.z << endl;

	os << planet.model_path << endl;
	os << planet.texture_paths.size () << endl;

	for (int i = 0; i < planet.texture_paths.size(); i++) {
		os << planet.texture_paths [i] << endl;
	}


	int num_children = planet.children.size ();

	os << num_children << endl;

	if (num_children == 0)
		return os;

	os << endl;

	for (int i = 0; i < num_children; i++) { 

		Moon* m = (Moon*)planet.children [i];

		if (m == NULL) {
			os << "Child " << i << " is null?\n";
			continue;
		}

		os << *m << endl;
	}

	return os;
}
