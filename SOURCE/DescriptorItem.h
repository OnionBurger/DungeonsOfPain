#ifndef _Descriptor_Item_h_
#define _Descriptor_Item_h_

#include "DescriptorWithTriggers.h"
#include "DescriptorWithStats.h"
#include <vector>

class DescriptorItem : public DescriptorWithStats, public DescriptorWithTriggers {
public:
	std::string handle, name, usage;
	unsigned genWeight;
	int score;
	std::string textureFileName;

	std::vector<std::vector<std::string>> equipSlots;

	std::string ammoSlot;

	int manaCost;
	std::string specific;

	bool stackable;
	unsigned stacks;

	bool winsGame;

private:
	void setDefaultValues();

	void applyEquipableChild(const pugi::xml_node &node);
	void apply(const pugi::xml_node &node);

private:
	DescriptorItem();

public:
	DescriptorItem(const pugi::xml_node &node);

	bool scoreInRange(int min, int max) const;

	static DescriptorItem* makeTrophyItemDesc();
};

#endif