#include "RoomDungeon.h"
#include "Map.h"
#include "GlobalTextureCont.h"
#include "DungeonGenerator.h"
#include "ArchitectMineBaron.h"
#include "ArchitectBossLevel.h"
#include "Descriptors.h"
#include "DescriptorLevel.h"
#include "Player.h"
#include "UIDungeon.h"
#include <Morph\Flow.h>

RoomDungeon::RoomDungeon(unsigned levelNumber, unsigned avatarIndex, Descriptors *D) {
	lvlNum = levelNumber;
	avatarInd = avatarIndex;
	
	desc = D;
	isDescOwner = true;
}

unsigned RoomDungeon::getNumberOfLevels() const {
	return desc->getDescLevelSize();
}

void RoomDungeon::onEnter() {
	Architect *arch;
	if (!isLastLevel()) arch = new ArchitectMineBaron(false);
	else arch = new ArchitectBossLevel();

	DungeonGenerator *gen = new DungeonGenerator(this, arch, desc);
	Map *map = gen->generate(lvlNum);

	UIDungeon *ui;
	if (lvlNum == 0) {
		ui = new UIDungeon(map->getTileSize(), getFlow()->getWindow()->getSize());
		addObject(ui);
	} else {
		ui = ((UIDungeon*)(getObjects(UIDungeon::T_UI_DUNGEON, true)[0]));
		ui->resetCameraMovements();
	}
	
	map->assignUIDungeon(ui);
	
	Player *player;
	if (lvlNum == 0) {
		player = new Player(desc->getDescAvatar(avatarInd), ui->getInventory(), desc->getEffects());
		addObject(player);
	} else {
		player = ((Player*)(getObjects(Player::T_PLAYER, true)[0]));
	}

	map->assignPlayer(player);

	delete gen;
	delete arch;

	addObject(map);
}

const std::string& RoomDungeon::getLookDoor() const {
	return desc->getDescLevel(lvlNum)->door;
}

const std::string& RoomDungeon::getLookFloor() const {
	return desc->getDescLevel(lvlNum)->floor;
}

const std::string& RoomDungeon::getLookWall() const {
	return desc->getDescLevel(lvlNum)->wall;
}

bool RoomDungeon::goToNextLevel() {
	if (isLastLevel()) return false;

	isDescOwner = false;
	changeRoom(new RoomDungeon(lvlNum + 1, avatarInd, desc));
	return true;
}

RoomDungeon::~RoomDungeon() {
	if (isDescOwner) delete desc;
}