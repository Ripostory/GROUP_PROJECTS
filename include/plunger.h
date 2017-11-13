
#ifndef INCLUDE_PLUNGER_H
#define INCLUDE_PLUNGER_H

#include "physObject.h"

#define PLUNGER_FORCE 1000

class Plunger : public PhysObject{
public:
	Plunger();
	Plunger(char key);
	~Plunger();
private:
	void keyboard(eventType);
	void Update(unsigned int dt);
	bool isActive;
	char key;
};



#endif /* INCLUDE_PLUNGER_H */
