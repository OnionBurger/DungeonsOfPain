#include "Effect.h"
#include "Target.h"
#include "WithTriggers.h"

Effect::Effect(const pugi::xml_node &node) {
	readBasicInfo(node);
}

void Effect::readBasicInfo(const pugi::xml_node &node) {
	handle = node.attribute("handle").as_string("");
	type = node.attribute("type").as_string("");
	target = node.attribute("target").as_string("");
}

void Effect::run(WithTriggers *activator) {
	if (!target.empty()) activator->setCurrentTarget(target);
	run(activator->getCurrentTarget());
}