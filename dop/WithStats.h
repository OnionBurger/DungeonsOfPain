#ifndef _With_Stats_h_
#define _With_Stats_h_

#include <string>
#include <vector>

class DescriptorWithStats;

class WithStats {
public:
	enum Stat {
		S_UNKNOWN, 
		S_MAX_HP, // maximum health points
		S_MAX_MP, // maximum mana points
		S_STR, // stength
		S_ACC, // accuracy
		S_P_RES, // physical resistance
		S_M_RES, // magical resistance
		S_DXT, // dexterity
		S_DED, // deadliness
		S_INT, //intelligence
		S_RANGE // attack range
	};

	static std::string statToString(Stat S);
	static Stat stringToStat(const std::string &str);

	typedef std::pair<Stat, int> StatVal;

protected:
	std::vector<StatVal> stats;

	bool ranged;	
	std::string missileTextureFileName;

	bool immobile;

protected:
	unsigned getStatsNum() const { return stats.size(); }
	Stat getStatByIndex(unsigned index) const { return stats[index].first; }
	int getStatValByIndex(unsigned index) const { return getStat(getStatByIndex(index)); }
	
public:
	WithStats(const DescriptorWithStats *desc);
	
	virtual void setStat(Stat S, int val);
	virtual int getStat(Stat S) const;
	
	virtual void setRanged(bool b) { ranged = b; }
	virtual bool isRanged() const { return ranged; }

	virtual std::string getMissileTextureFileName() const { return missileTextureFileName; }

	virtual bool isImmobile() const { return immobile; }

	virtual ~WithStats() {}
};

#endif