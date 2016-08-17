#include "Map.h"
#include "Tile.h"
#include <Morph\Room.h>
#include <Morph\Flow.h>
#include "Player.h"
#include "Enemy.h"
#include "Item.h"
#include "UIDungeon.h"
#include "DescriptorItem.h"
#include "FovCalc.h"
#include "EnemyCoordinator.h"
#include "RoomDungeon.h"
#include "RoomMain.h"
#include "Missile.h"
#include "Descriptors.h"
#include "Inventory.h"
#include "InventorySlot.h"

Map::Map(unsigned width, unsigned height, Descriptors *D, const std::string &floor) 
	: morph::ObjButton(0, false) {
	desc = D;

	tilesCntX = width;
	tilesCntY = height;
	tileWidth = 32.f;
	tileHeight = 32.f;

	tiles = new Tile**[tilesCntX];

	for (unsigned i = 0; i < tilesCntX; ++i) {
		tiles[i] = new Tile*[tilesCntY];

		for (unsigned j = 0; j < tilesCntY; ++j) {
			tiles[i][j] = new Tile(sf::Vector2f(i * tileWidth, j * tileHeight), floor);
		}
	}

	player = 0;
	uiDungeon = 0;

	fovCalc = new FovCalc(this);
	enemyCoordin = new EnemyCoordinator(this);

	awaitingTarget = 0;
}

void Map::onCreate() {
	morph::ObjButton::onCreate();

	for (unsigned i = 0; i < tilesCntX; ++i) {
		for (unsigned j = 0; j < tilesCntY; ++j) {
			getRoom()->addObject(tiles[i][j]);
		}
	}

	centerCameraOnPlayer();
	calcFov();
	uiDungeon->refresh();
}

void Map::setPlayerStart(unsigned x, unsigned y) {
	playerStartX = x;
	playerStartY = y;
	if (player) player->setPosition(sf::Vector2f(playerStartX * tileWidth, playerStartY * tileHeight));
}

void Map::assignPlayer(Player *P) {
	player = P;

	if (player) {
		player->setPosition(sf::Vector2f(playerStartX * tileWidth, playerStartY * tileHeight));
		player->setLoc(this, playerStartX, playerStartY);
		if (uiDungeon) {
			uiDungeon->showPlayer(player);
			uiDungeon->refresh();
		}
	}
}

bool Map::placeEnemy(Enemy *E, unsigned x, unsigned y) {
	if (E == 0 || tiles[x][y]->hasEnemy()) return false;

	tiles[x][y]->placeEnemy(E);
	E->setLoc(this, x, y);
	return true;
}

void Map::assignUIDungeon(UIDungeon *ui) {
	uiDungeon = ui;

	if (uiDungeon) {
		uiDungeon->setMap(this);

		if (player) {
			uiDungeon->showPlayer(player);
			uiDungeon->refresh();
		}
	}
}

const std::string& Map::getLookDoor() {
	return ((RoomDungeon*)getRoom())->getLookDoor();
}

const std::string& Map::getLookWall() {
	return ((RoomDungeon*)getRoom())->getLookWall();
}

void Map::centerCameraOnPlayer() {
	if (player == 0) return;

	sf::View view = getRoom()->getFlow()->getWindow()->getView();

	sf::Vector2f offset = sf::Vector2f(getPlayerX() * tileWidth, getPlayerY() * tileHeight);
	offset += sf::Vector2f(tileWidth / 2, tileHeight / 2);
	offset -= view.getCenter();
	offset.x += uiDungeon->getRightSideMargin() / 2;

	view.move(offset);
	getRoom()->getFlow()->getWindow()->setView(view);

	if (uiDungeon) uiDungeon->onCameraMove(offset);
}

void Map::unrevealTiles() {
	// TODO: optimize
	for (unsigned i = 0; i < tilesCntX; ++i) {
		for (unsigned j = 0; j < tilesCntY; ++j) {
			tiles[i][j]->setRevealed(false);
		}
	}
}

void Map::onEnemySee() {
	enemyCoordin->onEnemyActivated();
}

void Map::onClickNormal() {
	sf::Vector2f click =
		getRoom()->getFlow()->getWindow()->mapPixelToCoords(sf::Vector2i(getMouseX(), getMouseY()));

	if (click.x < 0 || click.y < 0) return;

	unsigned indexX = (unsigned)(click.x / tileWidth);
	if (indexX >= tilesCntX) return;

	unsigned indexY = (unsigned)(click.y / tileHeight);
	if (indexY >= tilesCntY) return;

	if (getMouseButton() == sf::Mouse::Left) {
		tiles[indexX][indexY]->showInfoWhenClicked(uiDungeon);
	}
	else if (getMouseButton() == sf::Mouse::Right) {
		if (player && tiles[indexX][indexY]->hasEnemy()) {
			if (playerAttacksEnemy(indexX, indexY)) enemyTurn();
			uiDungeon->refresh();
		}
		else {
			uiDungeon->showEnemy(0);
			uiDungeon->showItem(0);
		}
	}
}

void Map::onClickTargeting() {
	if (getMouseButton() == sf::Mouse::Left) {
		sf::Vector2f click =
			getRoom()->getFlow()->getWindow()->mapPixelToCoords(sf::Vector2i(getMouseX(), getMouseY()));

		if (click.x < 0 || click.y < 0) return;

		unsigned indexX = (unsigned)(click.x / tileWidth);
		if (indexX >= tilesCntX) return;

		unsigned indexY = (unsigned)(click.y / tileHeight);
		if (indexY >= tilesCntY) return;

		tiles[indexX][indexY]->showInfoWhenClicked(uiDungeon);

		if (awaitingTarget->setSpecTarget(tiles[indexX][indexY])) {
			if (player->spendManaFor(awaitingTarget)) {
				if (awaitingTarget->onTrigger(Trigger::T_USE)) {
					uiDungeon->getInventory()->removeAllOutlines();
					setAwaitingTarget(0);
					enemyTurn();
					uiDungeon->refresh();
				}
			} else {
				uiDungeon->playSound("low_mana.wav");
				uiDungeon->getInventory()->removeAllOutlines();
				setAwaitingTarget(0);
				uiDungeon->refresh();
			}
		}
	} else if (getMouseButton() == sf::Mouse::Right) {
		uiDungeon->getInventory()->removeAllOutlines();
		setAwaitingTarget(false);
	}
}

void Map::onClick() {
	isTargetingState() ? onClickTargeting() : onClickNormal();
}

void Map::onKeyPressed(const sf::Event::KeyEvent &e) {
	if (isMovementKey(e.code)) {
		if (movePlayer(e.code)) enemyTurn();
		centerCameraOnPlayer();
		uiDungeon->refresh();
	} else if (useItemForKey(e.code)) {
		// all done in if method
	} else if (isInteractKey(e.code)) {
		if (pickupItem()) enemyTurn();
		else descendToNextLevel();
		uiDungeon->refresh();
	} else if (isSkipTurnKey(e.code)) {
		enemyTurn();
		uiDungeon->refresh();
	} else if (isQuitKey(e.code)) {
		onQuitDungeon();
		changeRoom(new RoomMain());
	}
}

void Map::enemyTurn() {
	enemyCoordin->enemyTurn();
	if (player) player->onTrigger(Trigger::T_STEP);
}

bool Map::isMovementKey(const sf::Keyboard::Key &k) const {
	return k == sf::Keyboard::Up
		|| k == sf::Keyboard::Down
		|| k == sf::Keyboard::Left
		|| k == sf::Keyboard::Right
		|| k == sf::Keyboard::W
		|| k == sf::Keyboard::A
		|| k == sf::Keyboard::S
		|| k == sf::Keyboard::D;
}

bool Map::useItemForKey(const sf::Keyboard::Key &k) const {
	if (k == sf::Keyboard::Num1) {
		if (uiDungeon->getInventory()->getSlotCnt() >= 1) {
			uiDungeon->getInventory()->getSlot(0)->onLeftClick();
			return true;
		} else {
			return false;
		}
	} else if (k == sf::Keyboard::Num2) {
		if (uiDungeon->getInventory()->getSlotCnt() >= 2) {
			uiDungeon->getInventory()->getSlot(1)->onLeftClick();
			return true;
		} else {
			return false;
		}
	} else if (k == sf::Keyboard::Num3) {
		if (uiDungeon->getInventory()->getSlotCnt() >= 3) {
			uiDungeon->getInventory()->getSlot(2)->onLeftClick();
			return true;
		} else {
			return false;
		}
	} else if (k == sf::Keyboard::Num4) {
		if (uiDungeon->getInventory()->getSlotCnt() >= 4) {
			uiDungeon->getInventory()->getSlot(3)->onLeftClick();
			return true;
		} else {
			return false;
		}
	} else if (k == sf::Keyboard::Num5) {
		if (uiDungeon->getInventory()->getSlotCnt() >= 5) {
			uiDungeon->getInventory()->getSlot(4)->onLeftClick();
			return true;
		} else {
			return false;
		}
	} else if (k == sf::Keyboard::Num6) {
		if (uiDungeon->getInventory()->getSlotCnt() >= 6) {
			uiDungeon->getInventory()->getSlot(5)->onLeftClick();
			return true;
		} else {
			return false;
		}
	} else if (k == sf::Keyboard::Num7) {
		if (uiDungeon->getInventory()->getSlotCnt() >= 7) {
			uiDungeon->getInventory()->getSlot(6)->onLeftClick();
			return true;
		} else {
			return false;
		}
	} else if (k == sf::Keyboard::Num8) {
		if (uiDungeon->getInventory()->getSlotCnt() >= 8) {
			uiDungeon->getInventory()->getSlot(7)->onLeftClick();
			return true;
		} else {
			return false;
		}
	} else if (k == sf::Keyboard::Num9) {
		if (uiDungeon->getInventory()->getSlotCnt() >= 9) {
			uiDungeon->getInventory()->getSlot(8)->onLeftClick();
			return true;
		} else {
			return false;
		}
	} else if (k == sf::Keyboard::Num0) {
		if (uiDungeon->getInventory()->getSlotCnt() >= 10) {
			uiDungeon->getInventory()->getSlot(9)->onLeftClick();
			return true;
		} else {
			return false;
		}
	}

	return false;
}

bool Map::isInteractKey(const sf::Keyboard::Key &k) const {
	return k == sf::Keyboard::LControl || k == sf::Keyboard::RControl;
}

bool Map::isSkipTurnKey(const sf::Keyboard::Key &k) const {
	return k == sf::Keyboard::Space;
}

bool Map::isQuitKey(const sf::Keyboard::Key &k) const {
	return k == sf::Keyboard::Escape;
}

bool Map::movePlayer(const sf::Keyboard::Key &k) {
	if (player == 0) return false;

	unsigned nextX = getPlayerX(), nextY = getPlayerY();

	if (k == sf::Keyboard::Up || k == sf::Keyboard::W) {
		--nextY;
	}
	else if (k == sf::Keyboard::Down || k == sf::Keyboard::S) {
		++nextY;
	}
	else if (k == sf::Keyboard::Left || k == sf::Keyboard::A) {
		--nextX;
	}
	else if (k == sf::Keyboard::Right || k == sf::Keyboard::D) {
		++nextX;
	}

	if (nextX >= tilesCntX ||
		nextY >= tilesCntY) {
		return false;
	}

	if (tiles[nextX][nextY]->blocksMovement() && !tiles[nextX][nextY]->hasDoor()) {
		return false;
	}

	if (tiles[nextX][nextY]->hasDoor()) {
		tiles[nextX][nextY]->removeDoor();
		if (!tiles[nextX][nextY]->blocksMovement()) playerMoves(nextX, nextY);
		return true;
	} else if (tiles[nextX][nextY]->hasEnemy()) {
		return playerAttacksEnemy(nextX, nextY);
	} else if (player->isImmobile()) {
		return true;
	} else {
		return playerMoves(nextX, nextY);
	}
}

void Map::onEnemyDeath(Enemy *E) {
	if (E == 0) return;

	unsigned x = E->getLocX(), y = E->getLocY();

	if (uiDungeon->getEnemyShown() == E) uiDungeon->showEnemy(0);
	tiles[x][y]->removeEnemy(true);
	enemyCoordin->onEnemyDeactivated();

	// if it's the last enemy in the last level
	if (getRoom()->objCount(Enemy::T_ENEMY, true) <= 1 &&
		((RoomDungeon*)getRoom())->isLastLevel()) {
		DescriptorItem *desc = DescriptorItem::makeTrophyItemDesc();
		Item *trophy = new Item(desc, this, 0);
		delete desc;

		tiles[x][y]->placeItem(trophy);
	}
}

bool Map::playerAttacksEnemy(unsigned enemyX, unsigned enemyY) {
	if (enemyX >= tilesCntX || enemyY >= tilesCntY) return false;

	Enemy *enemy = tiles[enemyX][enemyY]->getEnemy();
	return playerAttacksEnemy(enemy);
}

bool Map::playerAttacksEnemy(Enemy *enemy) {
	if (player == 0) return false;
	if (enemy == 0) return false;

	uiDungeon->showEnemy(enemy);

	if (!tiles[enemy->getLocX()][enemy->getLocY()]->isRevealed()) return false;
	if (!player->inRangeForAttack(enemy->getLocX(), enemy->getLocY())) return false;

	if (!player->drainAmmoForAttack(tiles[enemy->getLocX()][enemy->getLocY()])) {
		uiDungeon->playSound("no_ammo.wav");
		return false;
	}

	if (player->isRanged()) {
		makeMissile(player, enemy, player->getMissileTextureFileName());
	}

	enemy->getAttacked(player);

	if (enemy->isDead()) {
		onEnemyDeath(enemy);
	}

	return true;
}

bool Map::playerMoves(unsigned toX, unsigned toY) {
	if (player == 0) return false;
	if (toX >= tilesCntX || toY >= tilesCntY) return false;

	player->setLoc(toX, toY);
	player->setPosition(getPlayerX() * tileWidth, getPlayerY() * tileHeight);
	calcFov();

	return true;
}

void Map::activeEnemyBeginsTurn(Enemy *enemy) {
	if (enemy == 0) return;
	
	enemy->onTrigger(Trigger::T_STEP);
}

bool Map::enemyAttacksPlayer(unsigned enemyX, unsigned enemyY) {
	if (enemyX >= tilesCntX || enemyY >= tilesCntY) return false;

	Enemy *enemy = tiles[enemyX][enemyY]->getEnemy();
	return enemyAttacksPlayer(enemy);
}

bool Map::enemyAttacksPlayer(Enemy *enemy) {
	if (player == 0) return false;
	if (enemy == 0) return false;

	if (enemy->isRanged())
		makeMissile(enemy, player, enemy->getMissileTextureFileName());

	player->getAttacked(enemy);

	if (player && player->isDead()) onPlayerDeath();

	return true;
}

void Map::makeMissile(Target *from, Target *to, const std::string &text) {
	getRoom()->addObject(new Missile(this, from, to, text));
}

void Map::onPlayerDeath() {
	if (player) {
		uiDungeon->showPlayer(0);
		getRoom()->destroyObject(player);
		player = 0;
	}
}

bool Map::enemyMoves(unsigned enemyX, unsigned enemyY, unsigned toX, unsigned toY) {
	if (enemyX >= tilesCntX || enemyY >= tilesCntY ||
		toX >= tilesCntX || toY >= tilesCntY) return false;

	Enemy *enemy = tiles[enemyX][enemyY]->getEnemy();
	return enemyMoves(enemy, toX, toY);
}

bool Map::enemyMoves(Enemy *enemy, unsigned toX, unsigned toY) {
	if (enemy == 0) return false;
	if (enemy->getLocX() == toX && enemy->getLocY() == toY) return false;
	if (enemy->isImmobile()) return false;

	if (tiles[toX][toY]->hasEnemy()) return false;
	if (player != 0 && toX == getPlayerX() && toY == getPlayerY()) return false;

	tiles[enemy->getLocX()][enemy->getLocY()]->removeEnemy(false);
	tiles[toX][toY]->placeEnemy(enemy);
	enemy->setLoc(toX, toY);
	return true;
}

void Map::calcFov() {
	fovCalc->calcFov();
}

bool Map::pickupItem() {
	if (player == 0) return false;

	Item *item = tiles[getPlayerX()][getPlayerY()]->takeItem();

	if (!player->pickupItem(item)) {
		tiles[getPlayerX()][getPlayerY()]->placeItem(item);
		return false;
	}

	return true;
}

bool Map::descendToNextLevel() {
	if (player == 0) return false;
	if (!tiles[getPlayerX()][getPlayerY()]->hasStairs()) return false;

	if (uiDungeon) {
		uiDungeon->onDescend();
	}

	return ((RoomDungeon*)getRoom())->goToNextLevel();
}

bool Map::dropItem(Item *item) {
	if (player == 0 || item == 0) return false;

	item->onDrop(player);
	tiles[getPlayerX()][getPlayerY()]->placeItem(item);
	//enemyTurn();
	uiDungeon->refresh();
	return true;
}

void Map::onQuitDungeon() {
	if (player) player->onQuitDungeon();
	if (uiDungeon) uiDungeon->onQuitDungeon();
}

void Map::winGame() {
	onQuitDungeon();
	changeRoom(new RoomMain());
}

bool Map::validIntCoords(int x, int y) const {
	return x >= 0 && x < (int)tilesCntX && y >= 0 && y < (int)tilesCntY;
}

unsigned Map::getPlayerX() const {
	return player->getLocX();
}

unsigned Map::getPlayerY() const {
	return player->getLocY();
}

bool Map::tileHasPlayer(Tile *tile) const {
	if (isPlayerDead()) return false;

	return tiles[getPlayerX()][getPlayerY()] == tile;
}

Map::~Map() {
	delete fovCalc;
	delete enemyCoordin;
}