#include "Descriptors.h"
#include "DescriptorLevel.h"
#include "DescriptorAvatar.h"
#include "DescriptorEnemy.h"
#include "DescriptorItem.h"
#include "DescriptorBuff.h"
#include "StorageEffects.h"
#include "pugixml.hpp"

Descriptors::Descriptors() {
	effects = new StorageEffects();
}

void Descriptors::readLevels() {
	pugi::xml_document doc_levels;
	doc_levels.load_file("rsrcs/xmls/levels.xml");

	for (pugi::xml_node node = doc_levels.first_child(); node; node = node.next_sibling()) {
		levels.push_back(new DescriptorLevel(node));
	}
}

void Descriptors::readAvatars() {
	pugi::xml_document doc_avatars;
	doc_avatars.load_file("rsrcs/xmls/avatars.xml");

	for (pugi::xml_node node = doc_avatars.first_child(); node; node = node.next_sibling()) {
		avatars.push_back(new DescriptorAvatar(node));
	}
}

void Descriptors::readEnemies() {
	pugi::xml_document doc_enemies;
	doc_enemies.load_file("rsrcs/xmls/enemies.xml");

	for (pugi::xml_node node = doc_enemies.first_child(); node; node = node.next_sibling()) {
		enemies.push_back(new DescriptorEnemy(node));
	}
}

void Descriptors::readItems() {
	pugi::xml_document doc_items;
	doc_items.load_file("rsrcs/xmls/items.xml");

	for (pugi::xml_node node = doc_items.first_child(); node; node = node.next_sibling()) {
		items.push_back(new DescriptorItem(node));
	}
}

void Descriptors::readBuffs() {
	pugi::xml_document doc_buffs;
	doc_buffs.load_file("rsrcs/xmls/buffs.xml");

	for (pugi::xml_node node = doc_buffs.first_child(); node; node = node.next_sibling()) {
		DescriptorBuff *desc = new DescriptorBuff(node);

		if (!desc->handle.empty()) {
			buffs.insert(std::make_pair(desc->handle, desc));
		} else {
			delete desc;
		}
	}
}

void Descriptors::read() {
	effects->read();
	readLevels();
	readAvatars();
	readEnemies();
	readItems();
	readBuffs();
}

const DescriptorLevel* Descriptors::getDescLevel(unsigned ind) const {
	return levels[ind];
}

DescriptorLevel* Descriptors::getDescLevel(unsigned ind) {
	return levels[ind];
}

const DescriptorAvatar* Descriptors::getDescAvatar(unsigned ind) const {
	return avatars[ind];
}

const DescriptorEnemy* Descriptors::getDescEnemy(unsigned ind) const {
	return enemies[ind];
}

DescriptorEnemy* Descriptors::getDescEnemy(unsigned ind) {
	return enemies[ind];
}

const DescriptorItem* Descriptors::getDescItem(unsigned ind) const {
	return items[ind];
}

DescriptorItem* Descriptors::getDescItem(unsigned ind) {
	return items[ind];
}

const DescriptorItem* Descriptors::getDescItem(const std::string &handle) const {
	for (unsigned i = 0; i < items.size(); ++i)
		if (items[i]->handle == handle)
			return items[i];

	return 0;
}

DescriptorItem* Descriptors::getDescItem(const std::string &handle) {
	for (unsigned i = 0; i < items.size(); ++i)
		if (items[i]->handle == handle)
			return items[i];

	return 0;
}

const DescriptorBuff* Descriptors::getDescBuff(const std::string &handle) const {
	try {
		return buffs.at(handle);
	} catch (const std::out_of_range&) {
		return 0;
	}
}

void Descriptors::clear() {
	effects->clear();

	for (unsigned i = 0; i < levels.size(); ++i)
		delete levels[i];

	levels.clear();

	for (unsigned i = 0; i < avatars.size(); ++i)
		delete avatars[i];

	avatars.clear();

	for (unsigned i = 0; i < enemies.size(); ++i)
		delete enemies[i];

	enemies.clear();

	
	for (unsigned i = 0; i < items.size(); ++i)
		delete items[i];

	items.clear();

	for (std::unordered_map<std::string, DescriptorBuff*>::iterator it = buffs.begin(); 
		it != buffs.end(); ++it)
		delete it->second;

	buffs.clear();
}

Descriptors::~Descriptors() {
	clear();
	delete effects;
}