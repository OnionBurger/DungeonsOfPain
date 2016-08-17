#include "Buff.h"
#include "DescriptorBuff.h"
#include "Descriptors.h"
#include "BuffHolder.h"

Buff::Duration::Duration(const DescriptorBuff::Duration &d) {
	type = Trigger::stringToTrigg(d.type);
	startVal = d.val;

	currVal = startVal;
}

Buff::Buff(const DescriptorBuff *descBuff, StorageEffects *S) : 
	WithStats(descBuff), WithTriggers(descBuff, S), 
	handle(descBuff->handle), 
	name(descBuff->name) {
	holder = 0;

	for (unsigned i = 0; i < descBuff->durations.size(); ++i) {
		if (descBuff->durations[i].type.empty()) continue;

		durations.push_back(Duration(descBuff->durations[i]));
	}
	over = false;
}

bool Buff::onTrigger(Trigger::Trigg tr) {
	bool ret = WithTriggers::onTrigger(tr);

	for (unsigned i = 0; i < durations.size(); ++i) {
		if (durations[i].type == tr) {
			durations[i].decrease();

			if (durations[i].isOver())
				over = true;

			ret = true;
			break;
		}
	}

	return ret;
}

void Buff::setCurrentTarget(const std::string &target) {
	if (holder == 0) currTarget = 0;
	
	if (target == "other") currTarget = holder->getOther();
	else if (target == "self") currTarget = holder->getSelf();
	else if (target == "player") currTarget = holder->getPlayer();
	else currTarget = 0;
}