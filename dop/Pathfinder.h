#ifndef _Pathfinder_h_
#define _Pathfinder_h_

#include <utility>
#include <vector>
#include <list>

class Map;
class EnemyCoordinator;
class Tile;

class Pathfinder {
public:
	typedef std::pair<unsigned, unsigned> Point;
private:
	typedef std::pair<Point, unsigned> PointWithDist;

private:
	Map *map;
	EnemyCoordinator *enemyCoordin;

	unsigned width, height;
	unsigned **distances;

	unsigned visitedMark;
	unsigned **visitedTokens;
	void resetVisitations();

	bool wasVisited(unsigned x, unsigned y) const;
	bool wasVisited(const Point P) const { return wasVisited(P.first, P.second); }

	Tile* getTile(unsigned x, unsigned y);
	Tile* getTile(const Point P) { return getTile(P.first, P.second); }

	bool addToQueue(std::list<PointWithDist> &queue, const PointWithDist &P);

	bool isOkStep(const Point &from, const Point &to);
	void addCandidate(const Point &cand, std::vector<Point> &vec, unsigned &minDist);

public:
	Pathfinder(Map *M, EnemyCoordinator *enemyCoordinator);

	void recalcDist();
	Point findNextStep(const Point P);

	~Pathfinder();
};

#endif