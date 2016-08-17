#include "Inventory.h"
#include "InventorySlot.h"
#include <Morph\Room.h>
#include "Item.h"
#include "UIDungeon.h"
#include "Map.h"
#include "Player.h"

Inventory::Inventory(const sf::Rect<float> &Border, 
	UIDungeon *UI, const sf::Vector2f &tileSize) : UIBox(Border) {
	setPersistent(true);
	
	ui = UI;

	int slotCnt = 12;

	float x = Border.left + 20;
	float y = Border.top + 20;
	for (int i = 0; i < slotCnt; ++i) {
		slots.push_back(new InventorySlot(UI, sf::Vector2f(x, y), getDepth(), tileSize));

		x += tileSize.x + 8;
		if (x + tileSize.x >= Border.left + Border.width) {
			x = Border.left + 20;
			y += tileSize.y + 8;
		}
	}
}

void Inventory::onCreate() {
	for (unsigned i = 0; i < slots.size(); ++i) {
		getRoom()->addObject(slots[i]);
	}

	ui->getMap()->getPlayer()->addStartingItems();
}

void Inventory::setMap(Map *M) {
	for (unsigned i = 0; i < slots.size(); ++i) {
		slots[i]->setMap(M);
	}
}

bool Inventory::isFull() const {
	for (unsigned i = 0; i < slots.size(); ++i) {
		if (slots[i]->isEmpty())
			return false;
	}

	return true;
}

bool Inventory::addItem(Item *item, bool trigger, bool autoequip) {
	if (item == 0) return false;

	bool added = false;

	if (item->isStackable()) {
		for (unsigned i = 0; i < slots.size() && !added; ++i) {
			if (!slots[i]->isEmpty() && slots[i]->getItem()->getHandle() == item->getHandle()) {
				added = slots[i]->incStackSize(item->getStackSize());
				if (added) {
					if (item->getRoom()) getRoom()->destroyObject(item);
					else delete item;

					ui->showItem(slots[i]->getItem());
					ui->refresh();
				}
			}
		}
	}

	if (added) return true;

	for (unsigned i = 0; i < slots.size() && !added; ++i) {
		if (slots[i]->isEmpty()) {
			added = slots[i]->placeItem(item, trigger);
			if (added) {
				if (item->getRoom() == 0) getRoom()->addObject(item);
				ui->showItem(item);
				ui->refresh();

				if (autoequip) {
					if (item->canEquip(this)) slots[i]->equip();
				}
			}
		}
	}

	return added;
}

void Inventory::removeAllOutlines() {
	for (unsigned i = 0; i < slots.size(); ++i) {
		slots[i]->removeOutline();
	}
	
	ui->getMap()->setAwaitingTarget(0);
}

void Inventory::onCameraMove(const sf::Vector2f &offset) {
	UIBox::onCameraMove(offset);

	for (unsigned i = 0; i < slots.size(); ++i) {
		slots[i]->onCameraMove(offset);
	}
}

void Inventory::refresh() {
}

void Inventory::onQuitDungeon() {
	for (unsigned i = 0; i < slots.size(); ++i) {
		slots[i]->onQuitDungeon();
	}

	setPersistent(false);
}