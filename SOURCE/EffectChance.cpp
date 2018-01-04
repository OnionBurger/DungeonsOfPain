#include "EffectChance.h"
#include "StorageEffects.h"
#include "WithTriggers.h"
#include "getRand.h"

EffectChance::EffectChance(StorageEffects *S, const pugi::xml_node &node) : Effect(node) {
	chance = node.attribute("chance").as_float(1.0f);
	if (chance < 0.0f) chance = 0.0f;
	else if (chance > 1.0f) chance = 1.0f;

	eff = S->getEffect(node.attribute("effect").as_string(""));
}

void EffectChance::run(WithTriggers *activator) {
	if (eff && chance > 0.0 && getRandFloat(1.0f) <= chance) {
		if (!target.empty()) activator->setCurrentTarget(target);
		eff->run(activator);
	}
}

void EffectChance::run(Target *T) {
	eff->run(T);
}