/*
 * event.h
 *
 *  Created on: Sep 4, 2017
 *      Author: Ronn Quijada
 *
 *      Broadcasts events
 */

#ifndef EVENT_H_
#define EVENT_H_

#include <vector>
#include "window.h"
using namespace std;

struct eventType {
	Uint32 eventVer;
	Uint8 mButton;
	SDL_Keycode key;
	Sint32 x;
	Sint32 y;
};

class event {
private:
	static vector<eventType> eventQueue;
	SDL_Event m_event;

	void pushEvent(Uint32, Uint8, SDL_Keycode, Sint32, Sint32);
public:
	void update();
	event();
	~event();
	eventType getEvent(int);
	int getSize();
};



#endif /* EVENT_H_ */
