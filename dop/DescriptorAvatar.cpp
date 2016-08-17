#include "DescriptorAvatar.h"

DescriptorAvatar::DescriptorAvatar(const pugi::xml_node &node) : DescriptorCombatant(node) {
	handle = node.attribute("handle").as_string("");
	name = node.attribute("name").as_string("");
	description = node.attribute("description").as_string("");

	for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling()) {
		apply(child);
	}
}

void DescriptorAvatar::apply(const pugi::xml_node &node) {
	if (strcmp(node.name(), "sprite") == 0) {
		textureFileName = node.attribute("file").as_string("");
	} else if (strcmp(node.name(), "starting_item") == 0) {
		startingItems.push_back(node.attribute("item").as_string());
	}
}