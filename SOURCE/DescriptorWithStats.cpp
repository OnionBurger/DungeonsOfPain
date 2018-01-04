#include "DescriptorWithStats.h"

DescriptorWithStats::DescriptorWithStats() {
	setDefaultValues();
}

DescriptorWithStats::DescriptorWithStats(const pugi::xml_node &node) {
	setDefaultValues();

	for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling()) {
		apply(child);
	}
}

void DescriptorWithStats::setDefaultValues() {
	ranged = false;
	immobile = false;
}

void DescriptorWithStats::apply(const pugi::xml_node &node) {
	if (strcmp(node.name(), "stat") == 0) {
		if (strcmp(node.attribute("name").as_string(), "range") == 0) {
			ranged = true;
			missileTextureFileName = node.attribute("missile").as_string("");
			stats.push_back(WithStats::StatVal(WithStats::S_RANGE, node.attribute("val").as_int(0)));
		} else {
			stats.push_back(WithStats::StatVal(WithStats::stringToStat(node.attribute("name").as_string()), 
				node.attribute("val").as_int(0)));
		}
	} else if (strcmp(node.name(), "immobile") == 0) {
		immobile = true;
	}
}