#ifndef _UI_Dungeon_h_
#define _UI_Dungeon_h_

#include <Morph\Objects\ObjButton.h>
#include <SFML\Audio.hpp>

class Map;

class Player;
class Enemy;
class Item;

class InfoPlayer;
class InfoEnemy;
class InfoItem;
class Inventory;

class UIDungeon : public morph::ObjButton {
public:
	static const morph::ObjType T_UI_DUNGEON;

private:
	sf::Sound sound;

	Map *map;

	InfoPlayer *infoPlayer;
	InfoEnemy *infoEnemy;
	InfoItem *infoItem;
	Inventory *inventory;

	float rightMargin;

public:
	UIDungeon(const sf::Vector2f &tileSize, const sf::Vector2u &screenSize);

	morph::ObjType getType() const { return T_UI_DUNGEON; }

	Map* getMap() { return map; }
	void setMap(Map *M);

	float getRightSideMargin() const { return rightMargin; }

	Inventory* getInventory() { return inventory; }

	void playSound(const std::string &soundFileName);

	void showPlayer(Player *player);
	void showEnemy(Enemy *enemy);
	void showItem(Item *item);

	Enemy* getEnemyShown();
	Item* getItemShown();

	bool inventoryIsFull() const;

	void onCameraMove(const sf::Vector2f &offset);
	void resetCameraMovements();

	void refresh();

	void onCreate();

	// also, prevents tiles from being clicked on through UI
	void onClick();

	void onDescend();
	void onQuitDungeon();
};

#endif