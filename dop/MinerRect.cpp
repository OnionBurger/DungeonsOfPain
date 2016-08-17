#include "MinerRect.h"
#include "getRand.h"

MinerRect::MinerRect(ArchitectMineBaron *A) : Miner(A) {
}

bool MinerRect::doMining(const Architect::t_point &gate) {
	if (!mineInitial(gate)) return false;

	plannedExpands = 4 + getRandUnsigned(5);

	while (plannedExpands > 0 && !sides.empty()) {
		unsigned sideIndex = pickSide();

		prepVarsForExp(sides[sideIndex]);
		if (canExpand(sides[sideIndex])) {
			expand(sides[sideIndex]);
			--plannedExpands;
		} else {
			removeSide(sideIndex);
		}
	}

	placeEnemiesAndItems();

	return true;
}

bool MinerRect::mineInitial(const Architect::t_point &gate) {
	if (mine(gate.first - 1, gate.second)) {
		topLeft = Architect::t_point(gate.first - 1, gate.second);
		sides.push_back(UP);
		sides.push_back(DOWN);
		sides.push_back(LEFT);
	} else if (mine(gate.first, gate.second - 1)) {
		topLeft = Architect::t_point(gate.first, gate.second - 1);
		sides.push_back(UP);
		sides.push_back(LEFT);
		sides.push_back(RIGHT);
	} else if (mine(gate.first + 1, gate.second)) {
		topLeft = Architect::t_point(gate.first + 1, gate.second);
		sides.push_back(UP);
		sides.push_back(DOWN);
		sides.push_back(RIGHT);
	} else if (mine(gate.first, gate.second + 1)) {
		topLeft = Architect::t_point(gate.first, gate.second + 1);
		sides.push_back(DOWN);
		sides.push_back(LEFT);
		sides.push_back(RIGHT);
	} else {
		return false;
	}
	
	currSize = Architect::t_point(1, 1);
	return true;
}

unsigned MinerRect::pickSide() const {
	return getRandUnsigned(sides.size());
}

void MinerRect::removeSide(unsigned index) {
	if (index + 1 < sides.size()) sides[index] = sides[sides.size() - 1];
	sides.pop_back();
}

void MinerRect::prepVarsForExp(Side s) {
	start = topLeft;
	inc = Architect::t_point(0, 0);

	if (s == UP) {
		start.second += -1;
		inc.first = 1;
		repeats = currSize.first;
	} else if (s == DOWN) {
		start.second += currSize.second;
		inc.first = 1;
		repeats = currSize.first;
	} else if (s == LEFT) {
		start.first += -1;
		inc.second = 1;
		repeats = currSize.second;
	} else if (s == RIGHT) {
		start.first += currSize.first;
		inc.second = 1;
		repeats = currSize.second;
	}
}

bool MinerRect::canExpand(Side s) {
	Architect::t_point iter = start;

	for (unsigned i = 0; i < repeats; ++i) {
		if (!arch->isFree(iter)) return false;

		iter.first += inc.first;
		iter.second += inc.second;
	}

	return true;
}

void MinerRect::expand(Side s) {
	Architect::t_point iter = start;
	
	for (unsigned i = 0; i < repeats; ++i) {
		mine(iter);

		iter.first += inc.first;
		iter.second += inc.second;
	}

	if (s == UP) --topLeft.second;
	else if (s == LEFT) --topLeft.first;

	currSize.first += inc.second;
	currSize.second += inc.first;
}

void MinerRect::placeEnemiesAndItems() {
	unsigned size = currSize.first * currSize.second;
	unsigned enemies = 1 + size / 9;
	unsigned items = 2 + size / 10;

	// some tiles may be picked multiple times, resulting in less enemies

	for (unsigned i = 0; i < enemies; ++i) {
		unsigned x, y;
		x = topLeft.first + getRandUnsigned(currSize.first);
		y = topLeft.second + getRandUnsigned(currSize.second);

		arch->placeEnemy(x, y);
	}

	for (unsigned i = 0; i < items; ++i) {
		unsigned x, y;
		x = topLeft.first + getRandUnsigned(currSize.first);
		y = topLeft.second + getRandUnsigned(currSize.second);

		arch->placeItem(x, y);
	}
}