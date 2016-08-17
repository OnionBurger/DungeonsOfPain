#ifndef _Architect_h_
#define _Architect_h_

#include <vector>

class Architect {
public:
	typedef std::pair<unsigned, unsigned> t_point;

	enum TileWith {
		T_EMPTY, 
		T_WALL, 
		T_DOOR
	};

protected:
	TileWith **lvl;
	unsigned sizeX, sizeY;

	void allocate();

	void fillWith(TileWith t);
	void fillWithWalls();

	t_point locPlayer, locStairs;
	std::vector<t_point> locsEnemy, locsItem;

	static void shuffle(std::vector<t_point> &vec);

public:
	Architect();

	virtual void generate() =0;

	unsigned getWidth() const { return sizeX; }
	unsigned getHeight() const { return sizeY; }

	TileWith getLvl(int x, int y) const { return lvl[x][y]; }
	TileWith getLvl(const t_point &loc) const { return lvl[loc.first][loc.second]; }

	t_point getLocPlayer() const { return locPlayer; }
	t_point getLocStairs() const { return locStairs; }

	unsigned getLocsEnemySize() const { return locsEnemy.size(); }
	t_point getLocEnemy(unsigned index) const { return locsEnemy[index]; }
	
	unsigned getLocsItemSize() const { return locsItem.size(); }
	t_point getLocItem(unsigned index) const { return locsItem[index]; }

	virtual ~Architect();
};

#endif