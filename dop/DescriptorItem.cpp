#include "DescriptorItem.h"

DescriptorItem::DescriptorItem(const pugi::xml_node &node) : 
	DescriptorWithStats(node),
	DescriptorWithTriggers(node) {
	setDefaultValues();

	handle = node.attribute("handle").as_string("");

	genWeight = node.attribute("gen_weight").as_uint(1);
	if (genWeight == 0) genWeight = 1;

	score = node.attribute("score").as_int(0);

	name = node.attribute("name").as_string("");

	usage = node.attribute("usage").as_string("");
	
	for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling()) {
		apply(child);
	}
}

void DescriptorItem::setDefaultValues() {
	stackable = false;
	stacks = 1;

	manaCost = 0;

	winsGame = false;
}

void DescriptorItem::apply(const pugi::xml_node &node) {
	std::string str(node.name());

	if (str == "sprite") {
		textureFileName = node.attribute("file").as_string("");
	} else if (str == "stacking") {
		stackable = node.attribute("stackable").as_bool(true);
		stacks = node.attribute("stacks").as_uint(1);
	} else if (str == "equipable") {
		if (usage == "") usage = "equipable";
		ammoSlot = node.attribute("ammo_slot").as_string("");

		for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling()) {
			applyEquipableChild(child);
		}
	} else if (str == "usable") {
		if (usage == "") usage = "usable";
		specific = node.attribute("specific").as_string("");
		manaCost = node.attribute("mana_cost").as_int(0);
	}
}

void DescriptorItem::applyEquipableChild(const pugi::xml_node &node) {
	std::string str(node.name());

	if (str == "slots") {
		std::vector<std::string> slots;

		for (pugi::xml_attribute attr : node.attributes()) {
			if (strcmp(attr.name(), "slot") == 0 && strlen(attr.as_string()) > 0) {
				slots.push_back(attr.as_string());
			}
		}

		equipSlots.push_back(slots);
	}
}

DescriptorItem::DescriptorItem() : DescriptorWithStats(), DescriptorWithTriggers() {
	setDefaultValues();
}

bool DescriptorItem::scoreInRange(int min, int max) const {
	// 0 means it can appear anywhere
	if (score == 0) return true;

	return score >= min && score <= max;
}

DescriptorItem* DescriptorItem::makeTrophyItemDesc() {
	DescriptorItem *desc = new DescriptorItem();
	desc->textureFileName = "trophy.png";
	desc->winsGame = true;
	return desc;
}