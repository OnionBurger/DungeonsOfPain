#include "ArchitectBossLevel.h"

ArchitectBossLevel::ArchitectBossLevel() {
}

void ArchitectBossLevel::generate() {
	sizeX = 13;
	sizeY = 17;
	allocate();
	fillWith(T_WALL);

	for (unsigned i = 2; i <= 10; ++i)
		for (unsigned j = 2; j <= 12; ++j)
			lvl[i][j] = T_EMPTY;

	for (unsigned i = 3; i <= 9; ++i) {
		lvl[i][1] = T_EMPTY;locsItem.push_back(t_point(i, 1));
	}

	locPlayer = t_point(6, 11);
	locsEnemy.push_back(t_point(6, 6));

	lvl[1][4] = T_EMPTY;locsItem.push_back(t_point(1, 4));
	lvl[11][4] = T_EMPTY;locsItem.push_back(t_point(11, 4));
	lvl[1][10] = T_EMPTY;locsItem.push_back(t_point(1, 10));
	lvl[11][10] = T_EMPTY;locsItem.push_back(t_point(11, 10));
	
	lvl[2][13] = T_EMPTY;locsItem.push_back(t_point(2, 13));
	lvl[9][13] = T_EMPTY;locsItem.push_back(t_point(9, 13));
	
	lvl[4][13] = T_EMPTY;lvl[4][14] = T_EMPTY;locsItem.push_back(t_point(4, 14));
	lvl[8][13] = T_EMPTY;lvl[8][14] = T_EMPTY;locsItem.push_back(t_point(8, 14));
	
	lvl[6][13] = T_EMPTY;lvl[6][14] = T_EMPTY;lvl[6][15] = T_EMPTY;locsItem.push_back(t_point(6, 15));
}