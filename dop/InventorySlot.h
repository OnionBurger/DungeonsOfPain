#ifndef _Inventory_Slot_h_
#define _Inventory_Slot_h_

#include <Morph\Objects\ObjButton.h>
#include <Morph\Looks\LookRectShape.h>

class Item;
class UIDungeon;
class Map;

class InventorySlot : public morph::ObjButton {
	UIDungeon *ui;

	morph::LookRectShape *rect;
	sf::Color colDefault, colEquip;

	Item *item;

public:
	InventorySlot(UIDungeon *UI, const sf::Vector2f &coords, int Depth, const sf::Vector2f &tileSize);
	
	void setMap(Map *M);

	bool placeItem(Item *I, bool trigger = true);
	bool incStackSize(unsigned n);
	bool dropItem() { return placeItem(0); }

	bool isEmpty() const { return item == 0; }
	Item* getItem() { return item; }

	void equip();
	void unequip();

	bool hasOutline() const;
	bool putOutline();
	bool removeOutline();

	void onLeftClick();
	void onRightClick();
	void onClick();

	void onCameraMove(const sf::Vector2f &offset);

	void onQuitDungeon();
};

#endif