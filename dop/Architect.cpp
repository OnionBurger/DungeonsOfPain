#include "Architect.h"
#include "getRand.h"

Architect::Architect() {
	sizeX = 0;
	sizeY = 0;
	lvl = 0;

	locPlayer = t_point(0, 0);
	locStairs = t_point(0, 0);
}

void Architect::allocate() {
	lvl = new TileWith*[sizeX];
	for (unsigned i = 0; i < sizeX; ++i)
		lvl[i] = new TileWith[sizeY];
}

void Architect::fillWith(TileWith t) {
	for (unsigned i = 0; i < sizeX; ++i) {
		for (unsigned j = 0; j < sizeY; ++j) {
			lvl[i][j] = t;
		}
	}
}

void Architect::fillWithWalls() {
	fillWith(T_WALL);
}

Architect::~Architect() {
	if (lvl) {
		for (unsigned i = 0; i < sizeX; ++i)
			delete [] lvl[i];

		delete [] lvl;

		lvl = 0;
	}
}

void Architect::shuffle(std::vector<t_point> &vec) {
	for (unsigned i = 1; i < vec.size(); ++i) {
		unsigned index = getRandUnsigned(i + 1);

		if (index != i) {
			t_point tmp = vec[index];
			vec[index] = vec[i];
			vec[i] = tmp;
		}
	}
}