#include "EffectCombine.h"
#include "StorageEffects.h"
#include "WithTriggers.h"

EffectCombine::EffectCombine(StorageEffects *S, const pugi::xml_node &node) : Effect(node) {
	for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling()) {
		if (strcmp(child.name(), "effect") == 0) {
			Effect *effect = S->getEffect(child.attribute("ref").as_string(""));
			std::string target = child.attribute("target").as_string("");

			if (effect) refs.push_back(EffectAndTarget(effect, target));
		}
	}
}

void EffectCombine::run(WithTriggers *activator) {
	if (!target.empty()) activator->setCurrentTarget(target);

	for (unsigned i = 0; i < refs.size(); ++i) {
		if (!refs[i].second.empty()) activator->setCurrentTarget(refs[i].second);
		refs[i].first->run(activator);
	}
}

void EffectCombine::run(Target *T) {
	for (unsigned i = 0; i < refs.size(); ++i) {
		refs[i].first->run(T);
	}
}