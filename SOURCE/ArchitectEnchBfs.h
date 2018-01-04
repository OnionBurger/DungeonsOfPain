#ifndef _ArchitectEnchBfs_h_
#define _ArchitectEnchBfs_h_

#include "Architect.h"

class ArchitectEnchBfs : public Architect {
	unsigned visits;
	float loopToler;

	t_point start;
	std::vector<std::pair<t_point, t_point>> queue;

	void chooseStart();
	std::pair<t_point, t_point> chooseNext();

	std::vector<t_point> freePlaces;

	void visit(const t_point &pnt);
	void visit(const std::pair<t_point, t_point> &pnt);

	void queueNeighbours(const t_point &pnt);
	void queueUp(const t_point &pnt1, const t_point &pnt2);

	bool getLoopTolerance();
	void visitRandomWalls();

public:
	ArchitectEnchBfs();

	void generate();

	void setLoopTolerancePercent(float p) { loopToler = p; }
	float getLoopTolerancePercent() const { return loopToler; }

	~ArchitectEnchBfs();
};

#endif