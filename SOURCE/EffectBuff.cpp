#include "EffectBuff.h"
#include "Target.h"

EffectBuff::EffectBuff(const pugi::xml_node &node) : Effect(node) {
	buff = node.attribute("buff").as_string("");
}

void EffectBuff::run(Target *T) {
	if (T) T->buff(this);
}