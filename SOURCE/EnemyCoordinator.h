#ifndef _Enemy_Coordinator_h_
#define _Enemy_Coordinator_h_

#include <vector>
#include "Pathfinder.h"

class Map;
class Enemy;

class EnemyCoordinator {
	friend class Pathfinder;

	Map *map;
	Pathfinder *pathfinder;

	unsigned activeEnemyCnt;

	std::vector<Pathfinder::Point> enemyLocs;
	unsigned getEnemyCnt() const { return activeEnemyCnt; }
	Pathfinder::Point getEnemyLoc(unsigned index) const { return enemyLocs[index]; }
	unsigned getEnemyLocsCnt() const { return enemyLocs.size(); }
	void clearEnemyLocs() { enemyLocs.clear(); }
	void addEnemyLoc(const Pathfinder::Point &P) { enemyLocs.push_back(P); }
	bool areAllEnemiesFound() const { return enemyLocs.size() == activeEnemyCnt; }

public:
	EnemyCoordinator(Map *M);

	void onEnemyActivated() { ++activeEnemyCnt; }
	void onEnemyDeactivated() { --activeEnemyCnt; }

	void enemyTurn();

	~EnemyCoordinator();
};

#endif