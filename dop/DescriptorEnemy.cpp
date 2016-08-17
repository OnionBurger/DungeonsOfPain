#include "DescriptorEnemy.h"

DescriptorEnemy::DescriptorEnemy(const pugi::xml_node &node) : DescriptorCombatant(node) {
	handle = node.attribute("handle").as_string("");

	genWeight = node.attribute("gen_weight").as_uint(1);
	if (genWeight == 0) genWeight = 1;

	diff = node.attribute("diff").as_int(0);

	name = node.attribute("name").as_string("");

	for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling()) {
		apply(child);
	}
}

void DescriptorEnemy::apply(const pugi::xml_node &node) {
	if (strcmp(node.name(), "sprite") == 0) {
		textureFileName = node.attribute("file").as_string("");
	} else if (strcmp(node.name(), "range") == 0) {
		missileTextureFileName = node.attribute("missile").as_string("");
	}
}

bool DescriptorEnemy::diffInRange(int min, int max) const {
	// 0 means it can appear anywhere
	if (diff == 0) return true;

	return diff >= min && diff <= max;
}