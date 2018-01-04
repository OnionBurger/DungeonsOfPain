#ifndef _Tile_h_
#define _Tile_h_

#include <Morph\Object.h>
#include <list>

class Wall;
class Door;
class Stairs;
class Enemy;
class Item;
class UIDungeon;

class Tile : public morph::Object {
	bool revealed;
	bool seen;

	Wall *wall;

	Door *door;

	Stairs *stairs;

	Enemy *enemy;

	std::list<Item*> items;
	std::list<Item*>::iterator currItem;

public:
	Tile(const sf::Vector2f &offset, const std::string &floor);

	void onCreate();

	void setRevealed(bool r);
	bool isRevealed() const { return revealed; }

	bool blocksLight() const;
	bool blocksMovement() const;

	void placeWall(const std::string &look);
	void removeWall();
	bool hasWall() const { return wall != 0; }

	void placeDoor(const std::string &look);
	void removeDoor();
	bool hasDoor() const { return door != 0; }

	void placeStairs();
	void removeStairs();
	bool hasStairs() const { return stairs != 0; }

	void placeEnemy(Enemy *E);
	void removeEnemy(bool shouldDestroyMorphObject);
	bool hasEnemy() const { return enemy != 0; }
	Enemy* getEnemy() { return enemy; }

	void placeItem(Item *I);
	Item* takeItem();
	bool hasItems() const { return !items.empty(); }
	Item* getNextItem();

	void showInfoWhenClicked(UIDungeon *ui);
};

#endif