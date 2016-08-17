#ifndef _Architect_Mine_Baron_h_
#define _Architect_Mine_Baron_h_

#include "Architect.h"

class Miner;

class ArchitectMineBaron : public Architect {
	bool lastLvl;

	int **occup; // -1 not occupied, 0 occupied by start hall, above zero occupied by sth else
	unsigned lastOccupToken;
	void allocateOccup();
	void clearOccup();

	std::vector<t_point> gateQueue;
	bool planGate(unsigned x, unsigned y);
	bool planGate(const t_point &pos) { return planGate(pos.first, pos.second); }

	void placeStartHall();

	Miner* pickMiner();
	t_point pickGate();
	void planGatesFromMiner(Miner *miner);

	void putDoor(const t_point &gate);

	void placeStairs();

	/*void placeStuff();
	std::vector<t_point> spawnPlaces;
	void gatherSpawnPlaces();
	void shuffleSpawnPlaces();*/

	void removeWaitingDoors();

public:
	ArchitectMineBaron(bool lastLevel);

	void generate();

	bool isUnoccupied(unsigned x, unsigned y) const;
	bool isUnoccupied(const t_point &pos) const { return isUnoccupied(pos.first, pos.second); }

	bool isFree(unsigned x, unsigned y) const;
	bool isFree(const t_point &pos) const { return isFree(pos.first, pos.second); }

	bool isEmpty(unsigned x, unsigned y) const { return lvl[x][y] == T_EMPTY; }
	bool isEmpty(const t_point &pos) const { return isEmpty(pos.first, pos.second); }

	bool digOut(unsigned x, unsigned y);
	bool digOut(const t_point &pos) { return digOut(pos.first, pos.second); }

	// take ownership of a wall
	bool ownWall(unsigned x, unsigned y);
	bool ownWall(const t_point &pos) { return ownWall(pos.first, pos.second); }

	bool placeEnemy(unsigned x, unsigned y);
	bool placeEnemy(const t_point &pos) { return placeEnemy(pos.first, pos.second); }

	bool placeItem(unsigned x, unsigned y);
	bool placeItem(const t_point &pos) { return placeItem(pos.first, pos.second); }

	~ArchitectMineBaron();
};

#endif