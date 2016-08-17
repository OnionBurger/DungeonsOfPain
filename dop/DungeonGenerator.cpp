#include "DungeonGenerator.h"
#include "Map.h"
#include "Tile.h"
#include "Item.h"
#include "Enemy.h"
#include "Player.h"
#include <ctime>
#include <cstdlib>
#include "getRand.h"
#include "Architect.h"
#include "Descriptors.h"
#include "DescriptorLevel.h"
#include "DescriptorItem.h"
#include "DescriptorEnemy.h"
#include "RoomDungeon.h"

DungeonGenerator::DungeonGenerator(RoomDungeon *room, Architect *arch, Descriptors *desc) {
	srand((unsigned)time(0));

	this->arch = arch;
	this->desc = desc;
	this->descLvl = desc->getDescLevel(room->getLevelNumber());

	lastLvl = room->isLastLevel();
}

Map* DungeonGenerator::generate(unsigned levelNumber) {
	arch->generate();

	sizeX = arch->getWidth();
	sizeY = arch->getHeight();

	Map *map = new Map(sizeX, sizeY, desc, descLvl->floor);

	placeWallsAndDoors(map, arch);
	placePlayer(map, arch);
	if (!lastLvl) placeStairs(map, arch);
	placeItems(map, arch);
	placeEnemies(map, arch);

	return map;
}

void DungeonGenerator::placeWallsAndDoors(Map *map, Architect *arch) {
	for (unsigned i = 0; i < sizeX; ++i) {
		for (unsigned j = 0; j < sizeY; ++j) {
			if (arch->getLvl(i, j) == Architect::T_WALL) {
				map->tiles[i][j]->placeWall(descLvl->wall);
			} else if (arch->getLvl(i, j) == Architect::T_DOOR) {
				map->tiles[i][j]->placeDoor(descLvl->door);
			}
		}
	}
}

void DungeonGenerator::placePlayer(Map *map, Architect *arch) {
	map->setPlayerStart(arch->getLocPlayer().first, 
		arch->getLocPlayer().second);
}

void DungeonGenerator::placeStairs(Map *map, Architect *arch) {
	map->tiles[arch->getLocStairs().first][arch->getLocStairs().second]
		->placeStairs();
}

// returns an index depending on weights, higher weight means greater chance of picking
unsigned roulette(const std::vector<unsigned> &weights, unsigned totalWeight) {
	unsigned pick = getRandUnsigned(totalWeight);

	for (unsigned i = 0; i < weights.size(); ++i) {
		if (pick < weights[i])
			return i;
		
		pick -= weights[i];
	}

	// this should not happen and should result in an error if it does
	return weights.size();
}

void DungeonGenerator::gatherItemDescs(std::vector<DescriptorItem*> &items) {
	itemGenWeights.clear();
	totalItemGenWeight = 0;

	for (unsigned i = 0; i < desc->getDescItemSize(); ++i) {
		DescriptorItem *item = desc->getDescItem(i);

		if (item->scoreInRange(descLvl->minScore, descLvl->maxScore)) {
			items.push_back(item);

			itemGenWeights.push_back(item->genWeight);
			totalItemGenWeight += item->genWeight;
		}
	}
}

DescriptorItem* DungeonGenerator::chooseItem(std::vector<DescriptorItem*> &items) {
	return items[roulette(itemGenWeights, totalItemGenWeight)];
}

void DungeonGenerator::placeItems(Map *map, Architect *arch) {
	std::vector<DescriptorItem*> items;
	gatherItemDescs(items);

	if (!items.empty()) {
		for (unsigned i = 0; i < arch->getLocsItemSize(); ++i) {
			Item *item = new Item(chooseItem(items), map, desc->getEffects());
			map->tiles[arch->getLocItem(i).first][arch->getLocItem(i).second]->placeItem(item);
		}
	}
}

void DungeonGenerator::gatherEnemyDescs(std::vector<DescriptorEnemy*> &enemies) {
	enemyGenWeights.clear();
	totalEnemyGenWeight = 0;

	for (unsigned i = 0; i < desc->getDescEnemySize(); ++i) {
		DescriptorEnemy *enemy = desc->getDescEnemy(i);

		if (enemy->diffInRange(descLvl->minDiff, descLvl->maxDiff)) {
			enemies.push_back(enemy);

			enemyGenWeights.push_back(enemy->genWeight);
			totalEnemyGenWeight += enemy->genWeight;
		}
	}
}

DescriptorEnemy* DungeonGenerator::chooseEnemy(std::vector<DescriptorEnemy*> &enemies) {
	return enemies[roulette(enemyGenWeights, totalEnemyGenWeight)];
}

void DungeonGenerator::placeEnemies(Map *map, Architect *arch) {
	std::vector<DescriptorEnemy*> enemies;
	gatherEnemyDescs(enemies);
	
	if (!enemies.empty()) {
		for (unsigned i = 0; i < arch->getLocsEnemySize(); ++i) {
			Enemy *enemy = new Enemy(chooseEnemy(enemies), desc->getEffects());
			map->placeEnemy(enemy, arch->getLocEnemy(i).first, arch->getLocEnemy(i).second);
		}
	}
}