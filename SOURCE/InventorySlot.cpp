#include "InventorySlot.h"
#include "Item.h"
#include "UIDungeon.h"
#include <Morph\CollMasks\CollRect.h>
#include "Map.h"
#include "Inventory.h"
#include "Player.h"

InventorySlot::InventorySlot(UIDungeon *UI, 
	const sf::Vector2f &coords, int Depth, 
	const sf::Vector2f &tileSize) : morph::ObjButton(coords, Depth) {
	setPersistent(true);

	ui = UI;
	item = 0;

	setCollMask(new morph::CollRect(tileSize));

	colDefault = sf::Color(50, 50, 50);
	colEquip = sf::Color(55, 160, 0);

	rect = new morph::LookRectShape(sf::Vector2f(tileSize.x + 4, tileSize.y + 4));
	rect->move(sf::Vector2f(-2, -2));
	rect->shape.setFillColor(colDefault);
	setLook(rect);
}

void InventorySlot::setMap(Map *M) {
	if (item) item->setMap(M);
}

void InventorySlot::equip() {
	if (item && !item->isEquipped()) {
		item->setEquipped(true);
		rect->shape.setFillColor(colEquip);
		item->onTrigger(Trigger::T_EQUIP);
		ui->getMap()->enemyTurn();
		ui->refresh();
	}
}

void InventorySlot::unequip() {
	if (item && item->isEquipped()) {
		item->setEquipped(false);
		rect->shape.setFillColor(colDefault);
		item->onTrigger(Trigger::T_UNEQUIP);
		//ui->getMap()->enemyTurn();
		ui->refresh();
	}
}

bool InventorySlot::placeItem(Item *I, bool trigger) {
	if (!isEmpty()) {
		if (I == 0) {
			item->removeOutline();
			ui->getMap()->setAwaitingTarget(0);
			unequip();
			if (trigger) item->onTrigger(Trigger::T_DROP);

			if (ui->getItemShown() == item) ui->showItem(0);
			item->setInventorySlot(0);
			item->setPersistent(false);
			item = 0;

			return true;
		} else {
			return false;
		}
	} else {
		if (I == 0) {
			return false;
		} else {
			item = I;

			item->setRevealed(true);
			item->setInventorySlot(this);
			item->setPosition(getPosition());
			item->setDepth(getDepth() - 1);
			item->setPersistent(true);

			if (trigger) item->onTrigger(Trigger::T_PICKUP);

			return true;
		}
	}
}

bool InventorySlot::incStackSize(unsigned n) {
	if (isEmpty() || !item->isStackable()) return false;

	item->incStackSize(n);
	return true;
}

bool InventorySlot::hasOutline() const {
	if (item == 0) return false;
	else return item->hasOutline();
}

bool InventorySlot::putOutline() {
	if (item == 0) return false;
	else {
		ui->getInventory()->removeAllOutlines();
		ui->getMap()->setAwaitingTarget(item);
		return item->putOutline(sf::Vector2f(-2, -2));
	}
}

bool InventorySlot::removeOutline() {
	if (item == 0) return false;
	else return item->removeOutline();
}

void InventorySlot::onLeftClick() {
	if (item) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
			ui->showItem(item);
		} else {
			if (ui->getMap()->isPlayerDead()) return;

			if (item->getUsage() == Item::U_USABLE) {
				if (item->getSpecType() != Item::S_NONE) {
					if (ui->getMap()->getPlayer()->hasManaFor(item)) {
						putOutline();
					} else {
						ui->playSound("low_mana.wav");
					}
				} else {
					if (ui->getMap()->getPlayer()->spendManaFor(item)) {
						if (item->onTrigger(Trigger::T_USE)) {
							ui->getMap()->enemyTurn();
							ui->refresh();
						}
					} else {
						ui->playSound("low_mana.wav");
					}
				}
			} else if (item->getUsage() == Item::U_EQUIPABLE) {
				if (item->canEquip(ui->getInventory())) {
					equip();
				} else {
					ui->playSound("cant_equip.wav");
				}
			}
		}
	}
}

void InventorySlot::onRightClick() {
	ui->getInventory()->removeAllOutlines();

	if (item) {
		if (ui->getMap()->isPlayerDead()) return;

		if (item->isEquipped()) {
			unequip();
		} else {
			Item *tmp = item;
			placeItem(0);
			ui->getMap()->dropItem(tmp);
		}
	}
}

void InventorySlot::onClick() {
	if (getMouseButton() == sf::Mouse::Left) {
		onLeftClick();
	} else if (getMouseButton() == sf::Mouse::Right) {
		onRightClick();
	}
}

void InventorySlot::onCameraMove(const sf::Vector2f &offset) {
	move(offset);
	if (item) item->move(offset);
}

void InventorySlot::onQuitDungeon() {
	if (item) item->setPersistent(false);

	setPersistent(false);
}