#include "Miner.h"

Miner::Miner(ArchitectMineBaron *A) {
	arch = A;
}

bool Miner::mine(unsigned x, unsigned y) {
	if (!arch->digOut(x, y)) return false;

	if (arch->ownWall(x - 1, y)) {
		gates.push_back(Architect::t_point(x - 1, y));
	}
	if (arch->ownWall(x + 1, y)) {
		gates.push_back(Architect::t_point(x + 1, y));
	}
	if (arch->ownWall(x, y - 1)) {
		gates.push_back(Architect::t_point(x, y - 1));
	}
	if (arch->ownWall(x, y + 1)) {
		gates.push_back(Architect::t_point(x, y + 1));
	}

	arch->ownWall(x - 1, y - 1);
	arch->ownWall(x + 1, y - 1);
	arch->ownWall(x - 1, y + 1);
	arch->ownWall(x + 1, y + 1);

	return true;
}