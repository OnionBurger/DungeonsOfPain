#ifndef _Miner_h_
#define _Miner_h_

#include "ArchitectMineBaron.h"

class Miner {
protected:
	ArchitectMineBaron *arch;

	std::vector<Architect::t_point> gates;
	
	bool mine(unsigned x, unsigned y);
	bool mine(const Architect::t_point &pos) { return mine(pos.first, pos.second); }

public:
	Miner(ArchitectMineBaron *A);

	virtual bool doMining(const Architect::t_point &gate) =0;
	std::vector<Architect::t_point>& getGates() { return gates; }

	virtual ~Miner() {}
};

#endif