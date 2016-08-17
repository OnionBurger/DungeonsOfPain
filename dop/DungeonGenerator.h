#ifndef _DungeonGenerator_h_
#define _DungeonGenerator_h_

#include <vector>

class Map;
class Architect;
class Descriptors;
class DescriptorLevel;
class DescriptorItem;
class DescriptorEnemy;
class RoomDungeon;

class DungeonGenerator {
	// not owned
	Architect *arch;
	Descriptors *desc;
	DescriptorLevel *descLvl;

	bool lastLvl;

	unsigned sizeX, sizeY;

	void placeWallsAndDoors(Map *map, Architect *arch);
	void placePlayer(Map *map, Architect *arch);
	void placeStairs(Map *map, Architect *arch);
	void placeItems(Map *map, Architect *arch);
	void placeEnemies(Map *map, Architect *arch);

	std::vector<unsigned> itemGenWeights;
	unsigned totalItemGenWeight;
	void gatherItemDescs(std::vector<DescriptorItem*> &items);
	DescriptorItem* chooseItem(std::vector<DescriptorItem*> &items);
	
	std::vector<unsigned> enemyGenWeights;
	unsigned totalEnemyGenWeight;
	void gatherEnemyDescs(std::vector<DescriptorEnemy*> &enemies);
	DescriptorEnemy* chooseEnemy(std::vector<DescriptorEnemy*> &enemies);

public:
	DungeonGenerator(RoomDungeon *room, Architect *arch, Descriptors *desc);

	Map* generate(unsigned levelNumber);
};

#endif