#ifndef _Menu_Item_Start_h_
#define _Menu_Item_Start_h_

#include "MenuItem.h"

class MenuItemStart : public MenuItem {
public:
	MenuItemStart(sf::Vector2f startCoords);

	void start();

	void onClick();
};

#endif