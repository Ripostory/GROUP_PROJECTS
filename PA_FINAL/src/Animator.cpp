
#include "Animator.h"

Animator::Animator()
{
}

Animator::~Animator()
{

}

void Animator::Update(unsigned int dt)
{
	//update front frame
	if (eventBuffer.size() != 0)
	{
		if (eventBuffer.front().Update(dt))
			eventBuffer.pop();
	}
}

void Animator::pushAnimation(AnimGroup animation)
{
	//try to push it into last animation frame
	if (eventBuffer.size() != 0)
	{
		if(!eventBuffer.back().addEvent(animation))
		{
			//if failed, create a new frame
			AnimFrame newFrame;
			newFrame.addEvent(animation);
			eventBuffer.push(newFrame);
		}
	}
	else
	{
		//empty, create a new frame
		AnimFrame newFrame;
		newFrame.addEvent(animation);
		eventBuffer.push(newFrame);
	}
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
}

AnimFrame::~AnimFrame()
{

}

bool AnimFrame::Update(unsigned int dt)
{
	int doneCounter = 0;
	std::vector<AnimGroup>::iterator it;
	for (it = children.begin(); it != children.end(); it++)
	{
		if(UpdateGroup(dt, &(*it)))
			doneCounter++;
	}

	if (children.size() == doneCounter)
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
	//check to make sure there isn't an existing animation on the same value
	if (children.size() != 0)
	{
		std::vector<AnimGroup>::iterator it;
		for (it = children.begin(); it != children.end(); it++)
		{
			if ((*it).id == group.id)
				return false;
		}
	}

	children.push_back(group);
	return true;
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
		break;
	case easeout:
		break;
	case easeinout:
		break;
	case exponential:
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

}

void AnimEvent::interpEaseOut(unsigned int dt)
{

}

void AnimEvent::interpEaseInOut(unsigned int dt)
{

}

void AnimEvent::interpExponential(unsigned int dt)
{

}








