#include "WithTriggers.h"
#include "Effect.h"
#include "StorageEffects.h"

WithTriggers::WithTriggers(const DescriptorWithTriggers *desc, StorageEffects *S) {
	addTriggers(desc->effects, S);
	currTarget = 0;
}

bool WithTriggers::onTrigger(Trigger::Trigg tr) {
	bool ret = false;

	for (unsigned i = 0; i < vec.size(); ++i) {
		if (vec[i].cause == tr && vec[i].effect != 0) {
			setCurrentTarget(vec[i].target);
			vec[i].effect->run(this);
			ret = true;
		}
	}

	return ret;
}

void WithTriggers::addTriggers(const std::vector<Trigger> &vec, StorageEffects *S) {
	if (S == 0) return;

	for (unsigned i = 0; i < vec.size(); ++i) {
		if (vec[i].cause == Trigger::T_UNDEFINED) continue;

		Trigger T = vec[i];

		T.effect = S->getEffect(vec[i].handle);
		if (T.effect) {
			addTrigger(T);
		}
	}
}