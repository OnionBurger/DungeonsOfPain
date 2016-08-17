#ifndef _With_Triggers_h_
#define _With_Triggers_h_

#include <utility>
#include <vector>
#include <string>
#include "DescriptorWithTriggers.h"

class Effect;
class Target;
class StorageEffects;

class WithTriggers {
	std::vector<Trigger> vec;

protected:
	Target *currTarget;

public:
	WithTriggers(const DescriptorWithTriggers *desc, StorageEffects *S);

	void addTrigger(const Trigger &E) { vec.push_back(E); }
	void addTriggers(const std::vector<Trigger> &vec, StorageEffects *S);

	virtual void setCurrentTarget(const std::string &target) {}
	Target* getCurrentTarget() { return currTarget; }

	bool onTrigger(Trigger::Trigg tr);

	virtual int getMagicStat() const { return 0; }

	virtual ~WithTriggers() {}
};

#endif