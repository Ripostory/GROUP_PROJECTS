

#ifndef INCLUDE_OCEAN_H_
#define INCLUDE_OCEAN_H_
#include "object.h"

class Ocean : public Object {
public:
	Ocean();
	~Ocean();
	void Update(unsigned int dt);
	void Render();
	float getOffset();
private:
	float offset;
};



#endif /* INCLUDE_OCEAN_H_ */
