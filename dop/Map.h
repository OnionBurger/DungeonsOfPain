#ifndef _Map_h_
#define _Map_h_

#include <Morph\Objects\ObjButton.h>

class Tile;
class Player;
class Enemy;
class Item;
class UIDungeon;
class Descriptors;
class Target;

class Map : public morph::ObjButton {
	friend class DungeonGenerator;
	friend class FovCalc;
	friend class EnemyCoordinator;
	friend class Pathfinder;

	Descriptors *desc;

	UIDungeon *uiDungeon;

	unsigned tilesCntX, tilesCntY;
	float tileWidth, tileHeight;
	Tile ***tiles;

	Player *player;
	unsigned playerStartX, playerStartY;

	void setPlayerStart(unsigned x, unsigned y);
	bool placeEnemy(Enemy *E, unsigned x, unsigned y);

	FovCalc *fovCalc;
	EnemyCoordinator *enemyCoordin;

	void centerCameraOnPlayer();

	void unrevealTiles();
	void calcFov();

	Tile* getTile(unsigned x, unsigned y) { return tiles[x][y]; }

	bool isMovementKey(const sf::Keyboard::Key &k) const;
	bool useItemForKey(const sf::Keyboard::Key &k) const;
	bool isInteractKey(const sf::Keyboard::Key &k) const;
	bool isSkipTurnKey(const sf::Keyboard::Key &k) const;
	bool isQuitKey(const sf::Keyboard::Key &k) const;

	bool movePlayer(const sf::Keyboard::Key &k);
	bool pickupItem();
	bool descendToNextLevel();

	bool validIntCoords(int x, int y) const;

	bool playerAttacksEnemy(unsigned enemyX, unsigned enemyY);
	bool playerAttacksEnemy(Enemy *enemy);
	bool playerMoves(unsigned toX, unsigned toY);
	void activeEnemyBeginsTurn(Enemy *enemy);
	bool enemyAttacksPlayer(unsigned enemyX, unsigned enemyY);
	bool enemyAttacksPlayer(Enemy *enemy);
	bool enemyMoves(unsigned enemyX, unsigned enemyY, unsigned toX, unsigned toY);
	bool enemyMoves(Enemy *enemy, unsigned toX, unsigned toY);

	void onClickNormal();
	void onClickTargeting();

	Item *awaitingTarget;

	void onQuitDungeon();

public:
	Map(unsigned width, unsigned height, Descriptors *D, const std::string &floor);
	
	void assignPlayer(Player *P);
	void assignUIDungeon(UIDungeon *ui);

	void onCreate();

	const std::string& getLookDoor();
	const std::string& getLookWall();

	void setAwaitingTarget(Item *I) { awaitingTarget = I; }
	bool isTargetingState() const { return awaitingTarget != 0; }

	void onClick();
	void onKeyPressed(const sf::Event::KeyEvent &e);

	void enemyTurn();
	
	bool dropItem(Item *item);

	void onEnemySee();

	void makeMissile(Target *from, Target *to, const std::string &text);

	void winGame();

	sf::Vector2f getTileSize() const { return sf::Vector2f(tileWidth, tileHeight); }

	const Descriptors* getDescriptors() const { return desc; }
	Descriptors* getDescriptors() { return desc; }
	
	void onEnemyDeath(Enemy *E);
	void onPlayerDeath();

	bool isPlayerDead() const { return player == 0; }
	unsigned getPlayerX() const;
	unsigned getPlayerY() const;
	Player* getPlayer() { return player; }

	bool tileHasPlayer(Tile *tile) const;

	~Map();
};

#endif