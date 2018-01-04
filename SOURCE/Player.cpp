#include "Player.h"
#include <Morph\Looks\LookSprite.h>
#include <Morph\Room.h>
#include "GlobalTextureCont.h"
#include "Enemy.h"
#include "Item.h"
#include "Inventory.h"
#include "InventorySlot.h"
#include "Map.h"
#include "DescriptorAvatar.h"
#include "DescriptorCombatant.h"
#include "Descriptors.h"
#include "Tile.h"
#include "getRand.h"

const morph::ObjType Player::T_PLAYER = 20;

Player::Player(const DescriptorAvatar *desc, Inventory *I, StorageEffects *S) : 
	morph::Object(10), Combatant(desc, S), 
	textureFileName(desc->textureFileName), 
	handle(desc->handle), 
	name(desc->name), 
	startingItems(desc->startingItems) {
	setPersistent(true);

	effects = S;

	inv = I;

	setLook(new morph::LookSprite(catchAvatarTexture(textureFileName), true));
}

int Player::getStat(Stat S) const {
	if (S == S_RANGE) return getRange();

	int ret = Combatant::getStat(S);

	for (unsigned i = 0; i < inv->slots.size(); ++i) {
		Item *item = inv->slots[i]->getItem();

		if (item && (item->getUsage() != Item::U_EQUIPABLE || item->isEquipped()))
			ret += item->getStat(S);
	}

	return ret;
}

int Player::getRange() const {
	int max = 0;

	if (Combatant::getRange() > max) max = Combatant::getRange();

	for (unsigned i = 0; i < inv->slots.size(); ++i) {
		Item *item = inv->slots[i]->getItem();

		if (item && (item->getUsage() != Item::U_EQUIPABLE || item->isEquipped()))
			if (item->getStat(S_RANGE) > max)
				max = item->getStat(S_RANGE);
	}

	return max;
}

bool Player::isRanged() const {
	if (Combatant::isRanged()) return true;

	for (unsigned i = 0; i < inv->slots.size(); ++i) {
		Item *item = inv->slots[i]->getItem();

		if (item && (item->getUsage() != Item::U_EQUIPABLE || item->isEquipped()))
			if (item->isRanged())
				return true;
	}

	return false;
}

bool Player::drainAmmoSlot(const std::string &ammo) {
	for (unsigned i = 0; i < inv->slots.size(); ++i) {
		Item *item = inv->slots[i]->getItem();

		if (item && item->isEquipped()) {
			if (item->hasEquipSlot(ammo) && item->incDestackCnt())
				return true;
		}
	}

	return false;
}

bool Player::drainAmmoForAttack(Tile *retrieveAmmoAt) {
	// did we give up cuz not enough ammo
	bool reset = false;

	// if item is equipped and needs ammo to attack with it
	// but not enough of that ammo type, then give up
	for (unsigned i = 0; i < inv->slots.size(); ++i) {
		Item *item = inv->slots[i]->getItem();

		if (item && item->isEquipped()) {
			std::string ammo = item->ammoSlot;
			if (!ammo.empty()) {
				reset = (drainAmmoSlot(item->ammoSlot) == false);
				if (reset) break;
			}
		}
	}

	if (reset) {
		// if we gave up return everything the way it was
		for (unsigned i = 0; i < inv->slots.size(); ++i) {
			Item *item = inv->slots[i]->getItem();

			if (item && item->isEquipped()) item->resetDestackCnt();
		}

		return false;
	} else {
		// otherwise, remove ammo
		// also, there is a chance of retrieving ammo
		for (unsigned i = 0; i < inv->slots.size(); ++i) {
			Item *item = inv->slots[i]->getItem();

			if (item && item->getDestackCnt() > 0) {
				if (getRandFloat(100) < getAmmoRetrChance()) {
					Item *retrieved = new Item(item->getDescriptor(), map, effects);
					retrieved->setStackSize(item->getDestackCnt());
					retrieveAmmoAt->placeItem(retrieved);
				}

				item->destack();
			}
		}

		return true;
	}
}

bool Player::hasManaFor(Item *item) {
	if (item == 0) return false;

	return currMP >= item->getManaCost();
}

bool Player::spendManaFor(Item *item) {
	if (!hasManaFor(item)) return false;

	changeCurrMP(-item->getManaCost());
	return true;
}

std::string Player::getMissileTextureFileName() const {
	std::string baseMissile(Combatant::getMissileTextureFileName());
	if (!baseMissile.empty()) return baseMissile;

	for (unsigned i = 0; i < inv->slots.size(); ++i) {
		Item *item = inv->slots[i]->getItem();

		if (item && (item->getUsage() != Item::U_EQUIPABLE || item->isEquipped()))
			if (item->isRanged())
				return item->getMissileTextureFileName();
	}

	return std::string();
}

bool Player::isImmobile() const {
	if (Combatant::isImmobile()) return true;

	for (unsigned i = 0; i < inv->slots.size(); ++i) {
		Item *item = inv->slots[i]->getItem();

		if (item && (item->getUsage() != Item::U_EQUIPABLE || item->isEquipped()))
			if (item->isImmobile())
				return true;
	}

	return false;
}

void Player::addStartingItems() {
	for (unsigned i = 0; i < startingItems.size(); ++i) {
		Item *item = new Item(map->getDescriptors()->getDescItem(startingItems[i]), map, map->getDescriptors()->getEffects());
		inv->addItem(item, false, true);
	}
}

bool Player::pickupItem(Item *item) {
	if (item == 0) return false;

	if (inv->addItem(item)) {
		item->onPickup(this);
		return true;
	} else {
		return false;
	}
}

bool Player::onTrigger(Trigger::Trigg tr) {
	bool ret = Combatant::onTrigger(tr);

	for (unsigned i = 0; i < inv->slots.size(); ++i) {
		Item *item = inv->slots[i]->getItem();

		if (item && (item->getUsage() != Item::U_EQUIPABLE || item->isEquipped())) {
			bool tmp = item->onTrigger(tr);
			ret = ret || tmp;
		}
	}

	return ret;
}

std::string Player::combatStatsToString() const {
	std::string txt;

	txt += "Health: " + std::to_string(currHP) + "/" + std::to_string(getStat(S_MAX_HP));
	txt += "\nMana: " + std::to_string(currMP) + "/" + std::to_string(getStat(S_MAX_MP));

	txt += "\nSTR: " + std::to_string(getStat(S_STR)) + " ACC: " + std::to_string(getStat(S_ACC));
	txt += "\nDXT: " + std::to_string(getStat(S_DXT)) + " DED: " + std::to_string(getStat(S_DED));
	txt += "\nINT: " + std::to_string(getStat(S_INT));
	txt += "\nP RES: " + std::to_string(getStat(S_P_RES)) + " M RES: " + std::to_string(getStat(S_M_RES));

	if (isRanged()) {
		txt += "\nRange: " + std::to_string(getRange());
	} else {
		txt += "\nRange: Melee";
	}

	return txt;
}

void Player::onDeath() {
	map->onPlayerDeath();
}