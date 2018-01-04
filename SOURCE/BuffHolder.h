#ifndef _Buff_Holder_h_
#define _Buff_Holder_h_

#include <list>
#include "Trigger.h"
#include "Buff.h"
#include "WithStats.h"

class Target;

class BuffHolder {
	std::list<Buff*> buffs;

	int getRange() const;

public:
	BuffHolder();

	void addBuff(Buff *B);

	virtual int getStat(WithStats::Stat S) const;
	virtual bool isRanged() const;
	virtual std::string getMissileTextureFileName() const;

	virtual bool isImmobile() const;

	bool onTrigger(Trigger::Trigg tr);

	virtual Target* getSelf() =0;
	virtual Target* getOther() =0;
	virtual Target* getPlayer() =0;

	~BuffHolder();
};

#endif