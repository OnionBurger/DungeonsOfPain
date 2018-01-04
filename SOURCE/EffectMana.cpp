#include "EffectMana.h"
#include "Target.h"

EffectMana::EffectMana(const pugi::xml_node &node) : Effect(node) {
	amount = node.attribute("val").as_int(0);
}

void EffectMana::run(Target *T) {
	if (T) T->mana(this);
}