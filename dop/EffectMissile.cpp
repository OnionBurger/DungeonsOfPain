#include "EffectMissile.h"
#include "WithTriggers.h"
#include "Target.h"

EffectMissile::EffectMissile(const pugi::xml_node &node) : Effect(node) {
	from = node.attribute("from").as_string("");
	missile = node.attribute("missile").as_string("");
}

void EffectMissile::run(WithTriggers *activator) {
	if (target.empty() || from.empty() || missile.empty()) return;

	activator->setCurrentTarget(from);
	src = activator->getCurrentTarget();

	activator->setCurrentTarget(target);
	run(activator->getCurrentTarget());
}

void EffectMissile::run(Target *T) {
	if (T && src) {
		T->missile(this);
	}
}