#include "EnemyCoordinator.h"
#include "Map.h"
#include "Enemy.h"
#include "Pathfinder.h"
#include "Tile.h"
#include "getRand.h"

const unsigned int chanceRangedMove = 50;

EnemyCoordinator::EnemyCoordinator(Map *M) {
	map = M;
	pathfinder = new Pathfinder(map, this);
	activeEnemyCnt = 0;
}

void EnemyCoordinator::enemyTurn() {
	if (activeEnemyCnt == 0) return;

	pathfinder->recalcDist();

	for (unsigned i = 0; !map->isPlayerDead() && i < enemyLocs.size(); ++i) {
		Enemy *enemy = map->getTile(enemyLocs[i].first, enemyLocs[i].second)->getEnemy();
		if (enemy == 0) continue;

		map->activeEnemyBeginsTurn(enemy);

		Pathfinder::Point next = pathfinder->findNextStep(enemyLocs[i]);

		if (enemy->canAttackPlayer()) {
			if (enemy->isRanged()) {
				if (enemy->coordDistance(map->getPlayerX(), map->getPlayerY()) > 2
						&& getRandUnsigned(100) < chanceRangedMove) {
					if (map->enemyMoves(enemy, next.first, next.second))
						enemyLocs[i] = next;
					else
						map->enemyAttacksPlayer(enemy);
				} else {
					map->enemyAttacksPlayer(enemy);
				}
			} else {
				map->enemyAttacksPlayer(enemy);
			}
		} else {
			if (map->enemyMoves(enemy, next.first, next.second))
				enemyLocs[i] = next;
		}
	}
}

EnemyCoordinator::~EnemyCoordinator() {
	delete pathfinder;
}