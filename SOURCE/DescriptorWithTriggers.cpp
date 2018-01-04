#include "DescriptorWithTriggers.h"

DescriptorWithTriggers::DescriptorWithTriggers(const pugi::xml_node &node) {
	for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling()) {
		apply(child);
	}
}

void DescriptorWithTriggers::apply(const pugi::xml_node &node) {
	std::string str(node.name());

	if (str == "effect") {
		Trigger T;

		T.cause = Trigger::stringToTrigg(node.attribute("trigger").as_string(""));
		T.handle = node.attribute("handle").as_string("");
		T.target = node.attribute("target").as_string("");

		effects.push_back(T);
	}
}

DescriptorWithTriggers::DescriptorWithTriggers() {
}