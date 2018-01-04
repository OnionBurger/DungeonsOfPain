#include "Tile.h"
#include <Morph\Looks\LookSprite.h>
#include "GlobalTextureCont.h"
#include "Wall.h"
#include "Door.h"
#include "Stairs.h"
#include "Enemy.h"
#include "Item.h"
#include <Morph\Room.h>
#include "UIDungeon.h"

Tile::Tile(const sf::Vector2f &offset, const std::string &floor) : morph::Object(offset, 20) {
	revealed = false;
	seen = false;

	wall = 0;
	door = 0;
	stairs = 0;
	enemy = 0;

	currItem = items.end();

	setRendered(false);
	setLook(new morph::LookSprite(catchFloorTexture(floor), true));
}

void Tile::onCreate() {
	if (wall) {
		getRoom()->addObject(wall);
	}
	
	if (door) {
		getRoom()->addObject(door);
	}
	
	if (stairs) {
		getRoom()->addObject(stairs);
	}

	if (enemy) {
		getRoom()->addObject(enemy);
	}

	if (items.size() > 0) {
		for (std::list<Item*>::iterator it = items.begin(); it != items.end(); ++it)
			getRoom()->addObject(*it);

		currItem = items.begin();
	}
}

void Tile::setRevealed(bool r) {
	if (revealed == r) return;

	revealed = r;

	if (revealed) {
		seen = true;
		setRendered(true);
		((morph::LookSprite*)(getLook()))->sprite.setColor(sf::Color(255, 255, 255));

		if (wall) wall->setRevealed(true);
		if (door) door->setRevealed(true);
		if (stairs) stairs->setRevealed(true);
		if (enemy) enemy->setRevealed(true);
		for (std::list<Item*>::iterator it = items.begin(); it != items.end(); ++it)
			(*it)->setRevealed(true);
	} else {
		((morph::LookSprite*)(getLook()))->sprite.setColor(sf::Color(255/2, 255/2, 255/2));

		if (wall) wall->setRevealed(false);
		if (door) door->setRevealed(false);
		if (stairs) stairs->setRevealed(false);
		if (enemy) enemy->setRevealed(false);
		for (std::list<Item*>::iterator it = items.begin(); it != items.end(); ++it)
			(*it)->setRevealed(false);
	}
}

bool Tile::blocksLight() const {
	return hasWall() || hasDoor();
}

bool Tile::blocksMovement() const {
	return hasWall() || hasDoor();
}

void Tile::placeWall(const std::string &look) {
	if (wall) {
		removeWall();
	}

	wall = new Wall(getPosition(), look);
	if (revealed) wall->setRevealed(true);
	if (getRoom()) getRoom()->addObject(wall);
}

void Tile::removeWall() {
	if (wall) {
		wall->setRendered(false);
		getRoom()->destroyObject(wall);
		wall = 0;
	}
}

void Tile::placeDoor(const std::string &look) {
	if (door) {
		removeDoor();
	}

	door = new Door(getPosition(), look);
	if (revealed) door->setRevealed(true);
	if (getRoom()) getRoom()->addObject(door);
}

void Tile::removeDoor() {
	if (door) {
		door->setRendered(false);
		getRoom()->destroyObject(door);
		door = 0;
	}
}

void Tile::placeStairs() {
	if (stairs) {
		removeStairs();
	}

	stairs = new Stairs(getPosition());
	if (revealed) stairs->setRevealed(true);
	if (getRoom()) getRoom()->addObject(stairs);
}

void Tile::removeStairs() {
	if (stairs) {
		stairs->setRendered(false);
		getRoom()->destroyObject(stairs);
		stairs = 0;
	}
}

void Tile::placeEnemy(Enemy *E) {
	if (enemy) {
		removeEnemy(true);
	}

	enemy = E;
	if (enemy) {
		enemy->setPosition(getPosition());
		enemy->setRevealed(revealed);
		if (getRoom() && enemy->getRoom() == 0) getRoom()->addObject(enemy);
	}
}

void Tile::removeEnemy(bool shouldDestroyMorphObject) {
	if (enemy) {
		if (shouldDestroyMorphObject) {
			enemy->setRendered(false);
			getRoom()->destroyObject(enemy);
		}
		enemy = 0;
	}
}

void Tile::placeItem(Item *I) {
	if (I) {
		I->setPosition(getPosition());
		I->setDepth(15);
		I->setRevealed(revealed);
		if (getRoom() && I->getRoom() == 0) getRoom()->addObject(I);

		items.push_back(I);
		currItem = --items.end();
	}
}

Item* Tile::takeItem() {
	if (currItem != items.end()) {
		Item *ret = *currItem;

		currItem = items.erase(currItem);
		
		if (currItem == items.end())
			currItem = items.begin();

		return ret;
	} else {
		return 0;
	}
}

Item* Tile::getNextItem() {
	if (currItem == items.end()) return 0;

	++currItem;
	if (currItem == items.end())
		currItem = items.begin();

	return currItem != items.end() ? *currItem : 0;
}

void Tile::showInfoWhenClicked(UIDungeon *ui) {
	if (enemy && enemy->isRevealed()) {
		ui->showEnemy(enemy);
	} else {
		Item *item = getNextItem();
		if (item && item->isKnown()) ui->showItem(item);
		else ui->showItem(0);
	}
}