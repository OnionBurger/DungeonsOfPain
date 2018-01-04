#include "StorageEffects.h"
#include "Effect.h"
#include "pugixml.hpp"
#include "EffectCombine.h"
#include "EffectChance.h"
#include "EffectDestroy.h"
#include "EffectHeal.h"
#include "EffectMana.h"
#include "EffectHurt.h"
#include "EffectSound.h"
#include "EffectMissile.h"
#include "EffectBuff.h"

StorageEffects::StorageEffects() {
}

void StorageEffects::read() {
	pugi::xml_document doc_items;
	doc_items.load_file("rsrcs/xmls/effects.xml");

	for (pugi::xml_node node = doc_items.first_child(); node; node = node.next_sibling()) {
		std::string type = node.attribute("type").as_string("");

		Effect *E = 0;

		if (type == "combine") {
			E = new EffectCombine(this, node);
		} else if (type == "chance") {
			E = new EffectChance(this, node);
		} else if (type == "destroy") {
			E = new EffectDestroy(node);
		} else if (type == "heal") {
			E = new EffectHeal(node);
		} else if (type == "mana") {
			E = new EffectMana(node);
		} else if (type == "hurt") {
			E = new EffectHurt(node);
		} else if (type == "sound") {
			E = new EffectSound(node);
		} else if (type == "missile") {
			E = new EffectMissile(node);
		} else if (type == "buff") {
			E = new EffectBuff(node);
		}

		if (E) {
			effects.insert(std::make_pair(E->getHandle(), E));
		}
	}
}

Effect* StorageEffects::getEffect(const std::string &handle) {
	try {
		return effects.at(handle);
	} catch (const std::out_of_range&) {
		return 0;
	}
}

void StorageEffects::clear() {
	for (std::unordered_map<std::string, Effect*>::iterator it = effects.begin(); 
		it != effects.end(); ++it)
		delete it->second;

	effects.clear();
}

StorageEffects::~StorageEffects() {
	clear();
}