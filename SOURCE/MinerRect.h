#ifndef _Miner_Rect_h_
#define _Miner_Rect_h_

#include "Miner.h"
#include <vector>

class MinerRect : public Miner {
	enum Side {
		UP, 
		DOWN, 
		LEFT, 
		RIGHT
	};

	unsigned plannedExpands;
	Architect::t_point topLeft, currSize;
	std::vector<Side> sides;

	bool mineInitial(const Architect::t_point &gate);

	unsigned pickSide() const;
	void removeSide(unsigned index);

	Architect::t_point start, inc;
	unsigned repeats;
	void prepVarsForExp(Side s);
	bool canExpand(Side s);
	void expand(Side s);

	void placeEnemiesAndItems();

public:
	MinerRect(ArchitectMineBaron *A);

	bool doMining(const Architect::t_point &gate);
};

#endif