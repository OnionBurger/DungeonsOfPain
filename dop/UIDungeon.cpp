#include "UIDungeon.h"
#include <Morph\Looks\LookRectShape.h>
#include <Morph\CollMasks\CollRect.h>
#include <Morph\Room.h>
#include "Map.h"
#include "InfoPlayer.h"
#include "InfoEnemy.h"
#include "InfoItem.h"
#include "Inventory.h"
#include "GlobalSoundBufferCont.h"

const morph::ObjType UIDungeon::T_UI_DUNGEON = 30;

UIDungeon::UIDungeon(const sf::Vector2f &tileSize, const sf::Vector2u &screenSize) 
	: morph::ObjButton(-10, true) {
	setPersistent(true);

	map = 0;

	rightMargin = 180.f;

	morph::CollRect *coll = new morph::CollRect(rightMargin, (float)screenSize.y);
	coll->move(sf::Vector2f((float)screenSize.x - rightMargin, 0));
	setCollMask(coll);

	morph::LookRectShape *rect = new morph::LookRectShape(sf::Vector2f(rightMargin, (float)screenSize.y));
	rect->shape.setFillColor(sf::Color::Black);
	rect->move(sf::Vector2f((float)screenSize.x - rightMargin, 0));
	setLook(rect);

	infoPlayer = new InfoPlayer(sf::Rect<float>(
		(float)screenSize.x - rightMargin, 
		0, 
		rightMargin, 
		(float)screenSize.y / 3));

	infoEnemy = new InfoEnemy(sf::Rect<float>(
		(float)screenSize.x - rightMargin, 
		(float)screenSize.y / 3, 
		rightMargin, 
		(float)screenSize.y / 3));

	infoItem = new InfoItem(sf::Rect<float>(
		(float)screenSize.x - rightMargin, 
		(float)screenSize.y / 3, 
		rightMargin, 
		(float)screenSize.y / 3));

	inventory = new Inventory(sf::Rect<float>(
		(float)screenSize.x - rightMargin, 
		2 * (float)screenSize.y / 3, 
		rightMargin, 
		(float)screenSize.y / 3), 
		this, tileSize);
}

void UIDungeon::setMap(Map *M) {
	map = M;
	inventory->setMap(map);
}

void UIDungeon::playSound(const std::string &soundFileName) {
	sound.setBuffer(catchSound(soundFileName));
	sound.play();
}

void UIDungeon::showPlayer(Player *player) {
	infoPlayer->setPlayer(player);
}

void UIDungeon::showEnemy(Enemy *enemy) {
	infoItem->setItem(0);
	infoEnemy->setEnemy(enemy);
}

void UIDungeon::showItem(Item *item) {
	infoEnemy->setEnemy(0);
	infoItem->setItem(item);
}

Enemy* UIDungeon::getEnemyShown() {
	return infoEnemy->getEnemy();
}

Item* UIDungeon::getItemShown() {
	return infoItem->getItem();
}

bool UIDungeon::inventoryIsFull() const {
	return inventory->isFull();
}

void UIDungeon::onCameraMove(const sf::Vector2f &offset) {
	move(offset);
	infoPlayer->onCameraMove(offset);
	infoEnemy->onCameraMove(offset);
	infoItem->onCameraMove(offset);
	inventory->onCameraMove(offset);
}

void UIDungeon::resetCameraMovements() {
	onCameraMove(-getPosition());
}

void UIDungeon::refresh() {
	infoPlayer->refresh();
	infoEnemy->refresh();
	infoItem->refresh();
	inventory->refresh();
}

void UIDungeon::onCreate() {
	morph::ObjButton::onCreate();

	getRoom()->addObject(infoPlayer);
	getRoom()->addObject(infoEnemy);
	getRoom()->addObject(infoItem);
	getRoom()->addObject(inventory);
}

void UIDungeon::onClick() {
	if (getMouseButton() == sf::Mouse::Right) {
		getInventory()->removeAllOutlines();
	}
}

void UIDungeon::onDescend() {
	map = 0;
	showItem(0);
	showEnemy(0);
}

void UIDungeon::onQuitDungeon() {
	infoPlayer->onQuitDungeon();
	infoEnemy->onQuitDungeon();
	infoItem->onQuitDungeon();
	inventory->onQuitDungeon();

	setPersistent(false);
}