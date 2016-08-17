#include "ArchitectMineBaron.h"
#include "getRand.h"
#include "MinerRect.h"

ArchitectMineBaron::ArchitectMineBaron(bool lastLevel) {
	lastLvl = lastLevel;
	occup = 0;
}

void ArchitectMineBaron::generate() {
	sizeX = 20;
	sizeY = 20;
	allocate();
	allocateOccup();
	fillWith(T_WALL);

	placeStartHall();
	++lastOccupToken;

	unsigned roomCnt = 0;
	unsigned consecutiveFails = 0;
	while (roomCnt < 8 && consecutiveFails < 5 && !gateQueue.empty()) {
		Miner *miner = pickMiner();
		t_point gate = pickGate();
		if (miner->doMining(gate)) {
			putDoor(gate);
			planGatesFromMiner(miner);
			++lastOccupToken;

			consecutiveFails = 0;
			++roomCnt;
		} else {
			++consecutiveFails;
		}
	}

	if (!lastLvl) placeStairs();

	removeWaitingDoors();
}

bool ArchitectMineBaron::digOut(unsigned x, unsigned y) {
	if (!isFree(x, y)) return false;

	lvl[x][y] = T_EMPTY;
	occup[x][y] = lastOccupToken;
	return true;
}

bool ArchitectMineBaron::isUnoccupied(unsigned x, unsigned y) const {
	return occup[x][y] < 0;
}

bool ArchitectMineBaron::isFree(unsigned x, unsigned y) const {
	if (x < 1 || y < 1 || x + 1 >= sizeX || y + 1 >= sizeY) return false;
	return isUnoccupied(x, y) || occup[x][y] == lastOccupToken;
}

bool ArchitectMineBaron::ownWall(unsigned x, unsigned y) {
	if (!isFree(x, y)) return false;
	if (lvl[x][y] != T_WALL) return false;

	occup[x][y] = lastOccupToken;
	return true;
}

bool ArchitectMineBaron::placeEnemy(unsigned x, unsigned y) {
	if (x < 1 || y < 1 || x + 1 >= sizeX || y + 1 >= sizeY) return false;
	if (occup[x][y] != lastOccupToken) return false;
	if (lvl[x][y] != T_EMPTY) return false;

	locsEnemy.push_back(t_point(x, y));
	return true;
}

bool ArchitectMineBaron::placeItem(unsigned x, unsigned y) {
	if (x < 1 || y < 1 || x + 1 >= sizeX || y + 1 >= sizeY) return false;
	if (occup[x][y] != lastOccupToken) return false;
	if (lvl[x][y] != T_EMPTY) return false;

	locsItem.push_back(t_point(x, y));
	return true;
}

bool ArchitectMineBaron::planGate(unsigned x, unsigned y) {
	if (x < 2 || y < 2 || x + 2 >= sizeX || y + 2 >= sizeY) return false;
	if (!isFree(x, y)) return false;
	if (lvl[x][y] != T_WALL) return false;

	gateQueue.push_back(t_point(x, y));
	occup[x][y] = lastOccupToken;
	return true;
}

void ArchitectMineBaron::placeStartHall() {
	t_point startHallPos;
	startHallPos.first = 1 + getRandUnsigned(sizeX - 2 - 5);
	startHallPos.second = 1 + getRandUnsigned(sizeY - 2 - 5);

	for (unsigned i = 1; i <= 3; ++i)
		for (unsigned j = 1; j <= 3; ++j)
			digOut(startHallPos.first + i, startHallPos.second + j);

	ownWall(startHallPos.first + 0, startHallPos.second + 0);
	ownWall(startHallPos.first + 0, startHallPos.second + 4);
	ownWall(startHallPos.first + 4, startHallPos.second + 0);
	ownWall(startHallPos.first + 4, startHallPos.second + 4);

	for (unsigned i = 1; i <= 3; ++i) {
		planGate(startHallPos.first + i, startHallPos.second + 0);
		planGate(startHallPos.first + i, startHallPos.second + 4);
		planGate(startHallPos.first + 0, startHallPos.second + i);
		planGate(startHallPos.first + 4, startHallPos.second + i);
	}

	locPlayer = t_point(startHallPos.first + 2, startHallPos.second + 2);
}

Miner* ArchitectMineBaron::pickMiner() {
	return new MinerRect(this);
}

Architect::t_point ArchitectMineBaron::pickGate() {
	unsigned ind = getRandUnsigned(gateQueue.size());
	t_point ret = gateQueue[ind];

	if (ind + 1 < gateQueue.size()) gateQueue[ind] = gateQueue[gateQueue.size() - 1];
	gateQueue.pop_back();

	return ret;
}

void ArchitectMineBaron::planGatesFromMiner(Miner *miner) {
	for (unsigned i = 0; i < miner->getGates().size(); ++i)
		planGate(miner->getGates()[i]);
}

void ArchitectMineBaron::putDoor(const t_point &gate) {
	lvl[gate.first][gate.second] = T_DOOR;
}

void ArchitectMineBaron::placeStairs() {
	if (locsItem.empty()) {
		locStairs = locPlayer;
	} else {
		unsigned index = getRandUnsigned(locsItem.size());
		locStairs = locsItem[index];

		if (index + 1 < locsItem.size()) locsItem[index] = locsItem[locsItem.size() - 1];
		locsItem.pop_back();
	}
}

/*void ArchitectMineBaron::placeStuff() {
	gatherSpawnPlaces();
	shuffleSpawnPlaces();

	if (spawnPlaces.empty()) {
		if (!lastLvl) locStairs = locPlayer;
		return;
	}

	if (!lastLvl) locStairs = spawnPlaces[0];

	unsigned enemies = 5, items = 3;

	for (unsigned i = 0; i < enemies; ++i)
		locsEnemy.push_back(spawnPlaces[1 + i]);

	
	for (unsigned i = 0; i < items; ++i)
		locsItem.push_back(spawnPlaces[1 + enemies + i]);
}

void ArchitectMineBaron::gatherSpawnPlaces() {
	// allocate an array of booleans
	bool **spawn;
	spawn = new bool*[sizeX];

	for (unsigned i = 0; i < sizeX; ++i) {
		spawn[i] = new bool[sizeY];

		// everything is good for spawning at first
		for (unsigned j = 0; j < sizeY; ++j) {
			spawn[i][j] = true;
		}
	}

	// uncheck walls, doors, everything outside of rooms and inside starting hall
	// also, we don't want anything to spawn adjacent to a door
	for (unsigned i = 0; i < sizeX; ++i) {
		for (unsigned j = 0; j < sizeY; ++j) {
			if (lvl[i][j] == T_DOOR) {
				spawn[i][j] = false;

				if (i > 0) spawn[i - 1][j] = false;
				if (i + 1 < sizeX) spawn[i + 1][j] = false;
				if (j > 0) spawn[i][j - 1] = false;
				if (j + 1 < sizeY) spawn[i][j + 1] = false;
			} else if (lvl[i][j] != T_EMPTY || occup[i][j] <= 0) {
				spawn[i][j] = false;
			}
		}
	}

	// now add those places to the vector
	for (unsigned i = 0; i < sizeX; ++i) {
		for (unsigned j = 0; j < sizeY; ++j) {
			if (spawn[i][j]) spawnPlaces.push_back(Architect::t_point(i, j));
		}
	}

	// delete the array of booleans
	for (unsigned i = 0; i < sizeX; ++i) delete [] spawn[i];
	delete [] spawn;
}

void ArchitectMineBaron::shuffleSpawnPlaces() {
	for (unsigned i = 1; i < spawnPlaces.size(); ++i) {
		unsigned index = getRandUnsigned(i + 1);

		if (index != i) {
			t_point tmp = spawnPlaces[index];
			spawnPlaces[index] = spawnPlaces[i];
			spawnPlaces[i] = tmp;
		}
	}
}*/

void ArchitectMineBaron::removeWaitingDoors() {
	for (unsigned i = 0; i < gateQueue.size(); ++i)
			lvl[gateQueue[i].first][gateQueue[i].second] = T_WALL;
}

void ArchitectMineBaron::allocateOccup() {
	occup = new int*[sizeX];
	lastOccupToken = 0;
	for (unsigned i = 0; i < sizeX; ++i) {
		occup[i] = new int[sizeY];
		for (unsigned j = 0; j < sizeY; ++j) {
			occup[i][j] = -1;
		}
	}
}

void ArchitectMineBaron::clearOccup() {
	if (occup) {
		for (unsigned i = 0; i < sizeX; ++i)
			delete [] occup[i];

		delete [] occup;

		lvl = 0;
	}
}

ArchitectMineBaron::~ArchitectMineBaron() {
	clearOccup();
}