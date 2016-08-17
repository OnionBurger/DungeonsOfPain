#include "DescriptorLevel.h"

DescriptorLevel::DescriptorLevel(const pugi::xml_node &node) {
	for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling()) {
		apply(child);
	}
}

void DescriptorLevel::setDefaultValues() {
	minScore = maxScore = 0;
	minDiff = maxDiff = 0;
}

void DescriptorLevel::apply(const pugi::xml_node &node) {
	if (strcmp(node.name(), "looks") == 0) {
		door = node.attribute("door").as_string("");
		floor = node.attribute("floor").as_string("");
		wall = node.attribute("wall").as_string("");
	} else if (strcmp(node.name(), "items") == 0) {
		minScore = node.attribute("min_score").as_int(0);
		maxScore = node.attribute("max_score").as_int(0);
	} else if (strcmp(node.name(), "enemies") == 0) {
		minDiff = node.attribute("min_diff").as_int(0);
		maxDiff = node.attribute("max_diff").as_int(0);
	}
}