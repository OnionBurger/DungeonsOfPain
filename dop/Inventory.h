#ifndef _Inventory_h_
#define _Inventory_h_

#include "UIBox.h"
#include <vector>

class InventorySlot;
class UIDungeon;
class Item;
class Map;

class Inventory : public UIBox {
	friend class Player;
	friend class Item;

	UIDungeon *ui;

	std::vector<InventorySlot*> slots;

public:
	Inventory(const sf::Rect<float> &Border, UIDungeon *UI, const sf::Vector2f &tileSize);

	void onCreate();
	
	void setMap(Map *M);

	unsigned getSlotCnt() const { return slots.size(); }
	InventorySlot* getSlot(unsigned index) { return slots[index]; }

	bool isFull() const;
	bool addItem(Item *item, bool trigger = true, bool autoequip = false);

	void removeAllOutlines();
	
	void onCameraMove(const sf::Vector2f &offset);
	void refresh();

	void onQuitDungeon();
};

#endif