
#ifndef INCLUDE_ANIMATOR_H_
#define INCLUDE_ANIMATOR_H_

#include "graphics_headers.h"
#include <vector>
#include <queue>

//Handles animating items

enum interpolation {
	none, linear, easein, easeout, easeinout, exponential, linearadd
};

//a single animated float
class AnimEvent {
public:
	AnimEvent();
	AnimEvent(float *toAnimate, float final, float time, interpolation lerp);
	~AnimEvent();
	bool Update(unsigned int dt);
private:
	float *value;
	float initial;
	float final;
	float time;
	float progress;
	interpolation lerp;
	bool isDone;

	void interpNone(unsigned int dt);
	void interpLinear(unsigned int dt);
	void interpEasein(unsigned int dt);
	void interpEaseOut(unsigned int dt);
	void interpEaseInOut(unsigned int dt);
	void interpExponential(unsigned int dt);

	void interpNoneAdd(unsigned int dt);
	void interpLinearAdd(unsigned int dt);
};

//a group of floats
//used to ensure multiple animations dont overlap one another
struct AnimGroup {
	int id = -1;
	std::vector<AnimEvent> event;
	AnimGroup(int i): id(i) {}
};

//an animation frame, updates all groups at the same time
class AnimFrame {
public:
	AnimFrame();
	AnimFrame(int id);
	~AnimFrame();
	bool Update(unsigned int dt);
	bool addEvent(AnimGroup);
	int getID();
	bool isFinished();
private:
	bool UpdateGroup(unsigned int dt, AnimGroup*);
	std::queue<AnimGroup> children;
	int id;
	bool done;
};

//the animator, animates each animation sequentially
class Animator {
public:
	Animator();
	~Animator();
	void Update(unsigned int dt);
	void interrupt(int id);
	bool isPending();
	bool isPending(int id);
	int getAnimationCount();

	//TODO add animator functions
	void animateFloat(float* value, float lerpTo, float time, interpolation interp, int id);
	void animateVec3(glm::vec3* value, glm::vec3 lerpTo, float time, interpolation interp, int id);
	void timer(float time, int id);

private:
	void pushAnimation(AnimGroup);
	std::vector<AnimFrame> eventBuffer;
	float nullValue;
};



#endif /* INCLUDE_ANIMATOR_H_ */
