#ifndef _Room_Main_h_
#define _Room_Main_h_

#include <Morph\Room.h>

class MenuItemStart;
class MenuItemQuit;

class RoomMain : public morph::Room {
	sf::Sprite title;

	MenuItemStart *menuItemStart;
	MenuItemQuit *menuItemQuit;

public:
	
	void onEnter();
	
	void postEvent(const sf::Event &e);
	void drawSelf(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif