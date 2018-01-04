#ifndef _Info_Item_h_
#define _Info_Item_h_

#include "UIBox.h"

class Item;

class InfoItem : public UIBox {
	Item *itemToShow;

public:
	InfoItem(const sf::Rect<float> &Border);

	void setItem(Item *item);
	Item* getItem() { return itemToShow; }
	
	void refresh();

	void onQuitDungeon() { setPersistent(false); }
};

#endif