
#ifndef SOLARSYSTEM_H_
#define SOLARSYSTEM_H_

#include <fstream>
#include "planet.h"
#include "moon.h"
#include "object.h"

class SolarSystem : public Object{
private:
    float angle;
    float rotationSpeed;
    void keyboard(eventType);

public:
    SolarSystem();
    SolarSystem(float, float);
    ~SolarSystem();
	void Update(unsigned int);

	void SaveSolSystem (string path);
	void LoadSolSystem(string filename);
	void LoadSolSystem (std::istream& is);

	void setSize(float);

	friend std::ostream& operator<< (std::ostream& os, const SolarSystem& planet);
	friend std::istream& operator>> (std::istream& is, SolarSystem& planet);
};



#endif /* SOLARSYSTEM_H_ */
