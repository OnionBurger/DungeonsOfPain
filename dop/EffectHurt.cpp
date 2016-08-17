#include "EffectHurt.h"
#include "WithTriggers.h"
#include "Target.h"

EffectHurt::EffectHurt(const pugi::xml_node &node) : Effect(node) {
	amount = node.attribute("val").as_int(0);
	magicCoef = node.attribute("int_coef").as_float(0);
}

void EffectHurt::run(WithTriggers *activator) {
	if (!target.empty()) activator->setCurrentTarget(target);
	if (magicCoef != 0) currMagicStat = activator->getMagicStat();
	run(activator->getCurrentTarget());
}

void EffectHurt::run(Target *T) {
	if (T) T->hurt(this);
}

int EffectHurt::getAmount() const {
	return (int)(amount + magicCoef * currMagicStat);
}