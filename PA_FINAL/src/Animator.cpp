
#include "Animator.h"

Animator::Animator()
{
}

Animator::~Animator()
{

}

void Animator::Update(unsigned int dt)
{
	//update all front frames
	if (eventBuffer.size() != 0)
	{
		std::vector<AnimFrame>::iterator it;
		int doneCounter = 0;

		for (it = eventBuffer.begin(); it != eventBuffer.end(); it++)
		{
			if ((&(*it))->Update(dt))
				doneCounter++;
		}

		if (eventBuffer.size() == doneCounter)
			eventBuffer.clear();
	}
}

void Animator::pushAnimation(AnimGroup animation)
{
	//search for frame
	std::vector<AnimFrame>::iterator it;
	for (it = eventBuffer.begin(); it != eventBuffer.end(); it++)
	{
		if ((*it).getID() == animation.id)
		{
			(&(*it))->addEvent(animation);
			return;
		}
	}

	//if there is no frame, add a new one
	AnimFrame newFrame(animation.id);
	newFrame.addEvent(animation);
	eventBuffer.push_back(newFrame);

}

bool Animator::isPending()
{
	if (eventBuffer.size() != 0)
		return true;
	else
		return false;
}

void Animator::interrupt(int id)
{
	std::vector<AnimFrame>::iterator it;
	for (it = eventBuffer.begin(); it != eventBuffer.end(); it++)
	{
		if ((*it).getID() == id)
		{
			eventBuffer.erase(it);
			return;
		}
	}
}

int Animator::getAnimationCount()
{
	return eventBuffer.size();
}

void Animator::animateFloat(float* value, float lerpTo, float time, interpolation interp, int id)
{
	AnimGroup final(id);
	final.event.push_back(AnimEvent(value, lerpTo, time, interp));
	pushAnimation(final);
}

void Animator::animateVec3(glm::vec3* value, glm::vec3 lerpTo, float time, interpolation interp, int id)
{
	AnimGroup final(id);
	final.event.push_back(AnimEvent(&value->x, lerpTo.x, time, interp));
	final.event.push_back(AnimEvent(&value->y, lerpTo.y, time, interp));
	final.event.push_back(AnimEvent(&value->z, lerpTo.z, time, interp));
	pushAnimation(final);
}

/*
 * =================
 * START OF AnimFrame CODE
 * =================
 */

AnimFrame::AnimFrame()
{
	id = -1;
}

AnimFrame::AnimFrame(int i)
{
	id = i;
}

AnimFrame::~AnimFrame()
{

}

bool AnimFrame::Update(unsigned int dt)
{
	//update front group if exists
	if (children.size() != 0)
		if (UpdateGroup(dt, &children.front()))
			children.pop();

	if (children.size() == 0)
		return true;
	return false;
}

bool AnimFrame::UpdateGroup(unsigned int dt, AnimGroup *set)
{
	std::vector<AnimEvent>::iterator it;
	int doneCounter = 0;

	for (it = set->event.begin(); it != set->event.end(); it++)
	{
		if ((&(*it))->Update(dt))
			doneCounter++;
	}

	if (set->event.size() == doneCounter)
		return true;

	return false;
}

bool AnimFrame::addEvent(AnimGroup group)
{
	//add to frame
	children.push(group);
	return true;
}

int AnimFrame::getID()
{
	return id;
}

/*
 * =================
 * START OF AnimEvent CODE
 * =================
 */

AnimEvent::AnimEvent()
{
	value = NULL;
	initial = 0;
	final = 0;
	time = 1;
	progress = 0;
	lerp = linear;
	isDone = false;
}

AnimEvent::AnimEvent(float *toAnimate, float finalval, float timeval, interpolation lerpval)
{
	value = toAnimate;
	initial = *value;
	final = finalval;
	time = timeval;
	progress = 0;
	lerp = lerpval;
	isDone = false;
}

AnimEvent::~AnimEvent()
{

}

bool AnimEvent::Update(unsigned int dt)
{
	//update current value (in case it was updated in a previous frame)
	if (progress == 0)
		initial = *value;

	if (!isDone)
	{
		progress += ((float) dt/1000.0f);
		if (progress/time >= 1)
		{
			isDone = true;
			if (lerp != linearadd)
				*value = final;
			return true;
		}
	}
	else
		return true;

	switch (lerp)
	{
	case none:
		interpNone(dt);
		break;
	case linear:
		interpLinear(dt);
		break;
	case easein:
		interpEasein(dt);
		break;
	case easeout:
		interpEaseOut(dt);
		break;
	case easeinout:
		interpEaseInOut(dt);
		break;
	case exponential:
		break;
	case linearadd:
		interpLinearAdd(dt);
		break;
	}
	return false;
}

//TODO implement all
void AnimEvent::interpNone(unsigned int dt)
{
	*value = final;
}

void AnimEvent::interpLinear(unsigned int dt)
{
	//update value
	*value = initial + (progress/time)*(final-initial);
}

void AnimEvent::interpEasein(unsigned int dt)
{
	*value = initial + pow((progress/time),2)*(final-initial);
}

void AnimEvent::interpEaseOut(unsigned int dt)
{
	*value = initial + (progress/time)*((progress/time) -2) * -(final-initial);
}

void AnimEvent::interpEaseInOut(unsigned int dt)
{
	float change = (progress/(time/2));
	if (change < 1.0)
		*value = initial + (final-initial)/2*change*change;
	else
	{
		change--;
		*value = -(final-initial)/2 * (change*(change-2) - 1) + initial;
	}
}

void AnimEvent::interpExponential(unsigned int dt)
{

}


void AnimEvent::interpNoneAdd(unsigned int dt)
{
	*value += final;
}

void AnimEvent::interpLinearAdd(unsigned int dt)
{
	//update value
	*value += (((float) dt/1000.0f)/time)*(final-initial);
}








