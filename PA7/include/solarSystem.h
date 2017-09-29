
#ifndef SOLARSYSTEM_H_
#define SOLARSYSTEM_H_

#include "planet.h"
#include "moon.h"
#include "object.h"

class SolarSystem : public Object{
private:
    float angle;
    float size;
    float rotationSpeed;
    void keyboard(eventType);
public:
    SolarSystem();
    SolarSystem(float, float);
    ~SolarSystem();
	void Update(unsigned int);
	void LoadSolSystem(string filename);

	void setSize(float);
};



#endif /* SOLARSYSTEM_H_ */
