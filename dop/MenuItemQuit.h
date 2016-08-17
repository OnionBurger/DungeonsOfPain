#ifndef _Menu_Item_Quit_h_
#define _Menu_Item_Quit_h_

#include "MenuItem.h"

class MenuItemQuit : public MenuItem {
public:
	MenuItemQuit(sf::Vector2f startCoords);

	void quit();

	void onClick();
};

#endif