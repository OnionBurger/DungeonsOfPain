#include "ArchitectEnchBfs.h"
#include "Tile.h"
#include "getRand.h"

ArchitectEnchBfs::ArchitectEnchBfs() : Architect() {
	sizeX = 20;
	sizeY = 20;

	visits = (sizeX * sizeY) / 4;
	loopToler = 80.f;
}

void ArchitectEnchBfs::generate() {
	allocate();
	fillWithWalls();

	chooseStart();
	visit(start);
	queueNeighbours(start);

	while (freePlaces.size() < visits && !queue.empty()) {
		std::pair<t_point, t_point> next = chooseNext();
		visit(next);
		queueNeighbours(next.first);
	}

	visitRandomWalls();

	shuffle(freePlaces);

	locPlayer = freePlaces[0];
	locStairs = freePlaces[1];
	for (unsigned i = 0; i < 5; ++i) locsEnemy.push_back(freePlaces[2 + i]);
	for (unsigned i = 0; i < 3; ++i) locsItem.push_back(freePlaces[2 + 5 + i]);
}

void ArchitectEnchBfs::chooseStart() {
	unsigned upper = sizeX % 2 == 0 ? (sizeX - 1) / 2 : sizeX / 2;
	start.first = 2 + 2 * getRandUnsigned(upper);

	upper = sizeY % 2 == 0 ? (sizeY - 1) / 2 : sizeY / 2;
	start.second = 2 + 2 * getRandUnsigned(upper);
}

std::pair<ArchitectEnchBfs::t_point, ArchitectEnchBfs::t_point> ArchitectEnchBfs::chooseNext() {
	unsigned index = getRandUnsigned(queue.size());

	std::pair<t_point, t_point> ret = queue[index];

	queue.erase(queue.begin() + index);

	return ret;
}

void ArchitectEnchBfs::visit(const t_point &pnt) {
	lvl[pnt.first][pnt.second] = T_EMPTY;
	freePlaces.push_back(t_point(pnt.first, pnt.second));
}

void ArchitectEnchBfs::visit(const std::pair<t_point, t_point> &pnt) {
	lvl[pnt.first.first][pnt.first.second] = T_EMPTY;
	freePlaces.push_back(t_point(pnt.first.first, pnt.first.second));

	lvl[pnt.second.first][pnt.second.second] = T_EMPTY;
	freePlaces.push_back(t_point(pnt.second.first, pnt.second.second));
}

void ArchitectEnchBfs::queueNeighbours(const t_point &pnt) {
	// algorithm will leave the outmost layer of walls standing
	// this way the level will be properly closed off by walls

	if (pnt.first >= 3)
		queueUp(t_point(pnt.first - 2, pnt.second), t_point(pnt.first - 1, pnt.second));

	if (pnt.first + 3 < sizeX)
		queueUp(t_point(pnt.first + 2, pnt.second), t_point(pnt.first + 1, pnt.second));
	
	if (pnt.second >= 3)
		queueUp(t_point(pnt.first, pnt.second - 2), t_point(pnt.first, pnt.second - 1));

	if (pnt.second + 3 < sizeY)
		queueUp(t_point(pnt.first, pnt.second + 2), t_point(pnt.first, pnt.second + 1));
}

void ArchitectEnchBfs::queueUp(const t_point &pnt1, const t_point &pnt2) {
	if (lvl[pnt1.first][pnt1.second] != T_WALL)
		return;

	for (unsigned i = 0; i < queue.size(); ++i) {
		if (pnt1.first == queue[i].first.first && pnt1.second == queue[i].first.second) {
			return;
		}
	}

	queue.push_back(std::pair<t_point, t_point>(pnt1, pnt2));
}

bool ArchitectEnchBfs::getLoopTolerance() {
	return getRandFloat(100) < loopToler;
}

void ArchitectEnchBfs::visitRandomWalls() {
	for (unsigned i = 1; i < sizeX - 1; i += 2) {
		for (unsigned j = 1; j < sizeY - 1; j += 2) {
			int cnt = 0;
			if (lvl[i-1][j] == T_EMPTY) ++cnt;
			if (i + 1 < sizeX && lvl[i+1][j] == T_EMPTY) ++cnt;
			if (lvl[i][j-1] == T_EMPTY) ++cnt;
			if (j + 1 < sizeY && lvl[i][j+1] == T_EMPTY) ++cnt;

			if (cnt > 0 && getLoopTolerance()) {
				visit(t_point(i, j));
			}
		}
	}
}

ArchitectEnchBfs::~ArchitectEnchBfs() {
}