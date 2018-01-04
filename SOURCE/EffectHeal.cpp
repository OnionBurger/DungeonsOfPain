#include "EffectHeal.h"
#include "Target.h"

EffectHeal::EffectHeal(const pugi::xml_node &node) : Effect(node) {
	amount = node.attribute("val").as_int(0);
}

void EffectHeal::run(Target *T) {
	if (T) T->heal(this);
}