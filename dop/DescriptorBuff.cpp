#include "DescriptorBuff.h"

DescriptorBuff::DescriptorBuff(const pugi::xml_node &node) : 
	DescriptorWithStats(node),
	DescriptorWithTriggers(node) {
	setDefaultValues();

	handle = node.attribute("handle").as_string("");
	name = node.attribute("name").as_string("");
	
	for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling()) {
		apply(child);
	}
}

void DescriptorBuff::setDefaultValues() {
}

DescriptorBuff::Duration::Duration(const pugi::xml_node &node) : 
	type(node.attribute("type").as_string("")), 
	val(node.attribute("val").as_uint(0)) {
}

void DescriptorBuff::apply(const pugi::xml_node &node) {
	std::string str(node.name());

	if (str == "duration") {
		durations.push_back(Duration(node));
	}
}