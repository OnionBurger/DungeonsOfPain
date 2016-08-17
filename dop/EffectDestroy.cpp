#include "EffectDestroy.h"
#include "Target.h"

EffectDestroy::EffectDestroy(const pugi::xml_node &node) : Effect(node) {
}

void EffectDestroy::run(Target *T) {
	if (T) T->destroy(this);
}