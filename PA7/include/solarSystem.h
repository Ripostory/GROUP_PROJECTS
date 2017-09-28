
#ifndef SOLARSYSTEM_H_
#define SOLARSYSTEM_H_

#include "planet.h"
#include "moon.h"
#include "object.h"

class SolarSystem : public Object{
private:
	event listener;

    float angle;
    float size;
    float rotationSpeed;
    void keyboard(eventType);
public:
    SolarSystem();
    SolarSystem(string, float, float);
    ~SolarSystem();
	void Update(unsigned int);

	void setSize(float);
};



#endif /* SOLARSYSTEM_H_ */
