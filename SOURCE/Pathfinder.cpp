#include "Pathfinder.h"
#include "Map.h"
#include "Tile.h"
#include "Enemy.h"
#include <climits>
#include "EnemyCoordinator.h"
#include "getRand.h"

Pathfinder::Pathfinder(Map *M, EnemyCoordinator *enemyCoordinator) {
	map = M;
	enemyCoordin = enemyCoordinator;

	width = map->tilesCntX;
	height = map->tilesCntY;

	distances = new unsigned*[width];
	for (unsigned i = 0; i < width; ++i)
		distances[i] = new unsigned[height];

	visitedTokens = new unsigned*[width];
	for (unsigned i = 0; i < width; ++i) {
		visitedTokens[i] = new unsigned[height];
	}

	resetVisitations();
}

void Pathfinder::resetVisitations() {
	visitedMark = 0;

	for (unsigned i = 0; i < width; ++i)
		for (unsigned j = 0; j < height; ++j)
			visitedTokens[i][j] = 0;
}

bool Pathfinder::wasVisited(unsigned x, unsigned y) const {
	if (x >= width || y >= height) return false;

	return visitedTokens[x][y] == visitedMark;
}

Tile* Pathfinder::getTile(unsigned x, unsigned y) {
	if (x >= width || y >= height) return 0;

	return map->tiles[x][y];
}

bool Pathfinder::addToQueue(std::list<PointWithDist> &queue, const PointWithDist &P) {
	if (P.first.first < width && P.first.second < height && 
		getTile(P.first) && !getTile(P.first)->blocksMovement() && 
		(!wasVisited(P.first) || P.second < distances[P.first.first][P.first.second])) {
		queue.push_back(P);
		return true;
	} else {
		return false;
	}
}

void Pathfinder::recalcDist() {
	if (map->player == 0) return;

	if (enemyCoordin->getEnemyCnt() == 0) return; // if there are no enemies to find paths for
	enemyCoordin->clearEnemyLocs();

	if (visitedMark == UINT_MAX) resetVisitations();
	++visitedMark;

	// an assumption is that distances won't be TOO big and will fit in the unsigned type limits
	std::list<PointWithDist> queue;
	std::list<PointWithDist>::iterator iter;

	queue.push_back(PointWithDist(Point(map->getPlayerX(), map->getPlayerY()), 0));
	iter = queue.begin();

	unsigned minInQueue = (*iter).second;
	unsigned maxInEnemyLocs;

	while (!queue.empty()) {
		PointWithDist curr = *iter;

		if (!wasVisited(curr.first) || curr.second < distances[curr.first.first][curr.first.second]) {
			distances[curr.first.first][curr.first.second] = curr.second;

			unsigned distOver = curr.second;

			if (!wasVisited(curr.first) && 
				getTile(curr.first) && 
				getTile(curr.first)->hasEnemy() && 
				getTile(curr.first)->getEnemy()->isSeen()) {
				if (enemyCoordin->getEnemyLocsCnt() == 0 || maxInEnemyLocs < curr.second) {
					maxInEnemyLocs = curr.second;
				}

				enemyCoordin->addEnemyLoc(curr.first);

				distOver += 10;
			} else {
				distOver += 1;
			}
			
			visitedTokens[curr.first.first][curr.first.second] = visitedMark;

			if (curr.first.first > 0 && 
				addToQueue(queue, PointWithDist(Point(curr.first.first - 1, curr.first.second), distOver))) {
				if (distOver < minInQueue) minInQueue = distOver;
			}
			
			if (addToQueue(queue, PointWithDist(Point(curr.first.first + 1, curr.first.second), distOver))) {
				if (distOver < minInQueue) minInQueue = distOver;
			}
			
			if (curr.first.second > 0 && 
				addToQueue(queue, PointWithDist(Point(curr.first.first, curr.first.second - 1), distOver))) {
				if (distOver < minInQueue) minInQueue = distOver;
			}
			
			if (addToQueue(queue, PointWithDist(Point(curr.first.first, curr.first.second + 1), distOver))) {
				if (distOver < minInQueue) minInQueue = distOver;
			}
		}
		
		iter = queue.erase(iter);

		if (curr.second == minInQueue && !queue.empty()) {
			std::list<PointWithDist>::iterator minIter = queue.begin();
			minInQueue = (*minIter).second;

			for (++minIter; minIter != queue.end(); ++minIter) {
				if (minInQueue > (*minIter).second) minInQueue = (*minIter).second;
			}
		}

		if (enemyCoordin->areAllEnemiesFound()) {
			if (maxInEnemyLocs < minInQueue) break;
		}
	}

	/* For debugging:
	unsigned mat1[20][20], mat2[20][20];
	for (unsigned i = 0; i < 20; ++i)
		for (unsigned j = 0; j < 20; ++j) {
			mat1[i][j] = distances[i][j];
			mat2[i][j] = visitedTokens[i][j];
		}

	int x = 5;
	++x;
	*/
}

bool Pathfinder::isOkStep(const Point &from, const Point &to) {
	if (from.first >= width || from.second >= height || 
		to.first >= width || to.second >= height) return false;

	return wasVisited(to) && !getTile(to)->hasEnemy() && 
		distances[to.first][to.second] <= distances[from.first][from.second];
}

void Pathfinder::addCandidate(const Point &cand, std::vector<Point> &vec, unsigned &minDist) {
	if (distances[cand.first][cand.second] == minDist) {
		vec.push_back(cand);
	}
	else if (distances[cand.first][cand.second] < minDist) {
		vec.clear();
		vec.push_back(cand);
		minDist = distances[cand.first][cand.second];
	}
}

Pathfinder::Point Pathfinder::findNextStep(const Point P) {
	std::vector<Point> candidates;

	unsigned minDist = UINT_MAX;
	if (P.first > 0 && isOkStep(P, Point(P.first - 1, P.second))) {
		addCandidate(Point(P.first - 1, P.second), candidates, minDist);
	}
	if (P.first < UINT_MAX && isOkStep(P, Point(P.first + 1, P.second))) {
		addCandidate(Point(P.first + 1, P.second), candidates, minDist);
	}
	if (P.second > 0 && isOkStep(P, Point(P.first, P.second - 1))) {
		addCandidate(Point(P.first, P.second - 1), candidates, minDist);
	}
	if (P.second < UINT_MAX && isOkStep(P, Point(P.first, P.second + 1))) {
		addCandidate(Point(P.first, P.second + 1), candidates, minDist);
	}

	if (candidates.empty()) return P;

	return candidates[getRandUnsigned(candidates.size())];
}

Pathfinder::~Pathfinder() {
	for (unsigned i = 0; i < width; ++i)
		delete[] distances[i];
	delete[] distances;

	for (unsigned i = 0; i < width; ++i)
		delete[] visitedTokens[i];
	delete[] visitedTokens;
}