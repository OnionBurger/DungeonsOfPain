#include "Item.h"
#include <Morph\Looks\LookMulti.h>
#include <Morph\Room.h>
#include "GlobalTextureCont.h"
#include "GlobalFontCont.h"
#include "Player.h"
#include "DescriptorItem.h"
#include "Map.h"
#include "InventorySlot.h"
#include "Inventory.h"
#include <algorithm>
#include "Tile.h"
#include "Enemy.h"
#include "EffectMissile.h"

const int stacksTextCharSize = 12;

Item::Usage Item::stringToUsage(const std::string &str) {
	if (str == "equipable") return U_EQUIPABLE;
	else if (str == "usable") return U_USABLE;
	else if (str == "none") return U_NONE;
	else return U_UNKNOWN;
}

Item::SpecType Item::stringToSpecType(const std::string &str) {
	if (str == "enemy") return S_ENEMY;
	else if (str == "character") return S_CHARACTER;
	else return S_NONE;
}

Item::Item(DescriptorItem *desc, Map *M, StorageEffects *S) : 
	morph::Object(), WithStats(desc), WithTriggers(desc, S), 
	textureFileName(desc->textureFileName), 
	handle(desc->handle), 
	name(desc->name), 
	equipSlots(desc->equipSlots), 
	ammoSlot(desc->ammoSlot) {
	this->desc = desc;

	invSlot = 0;
	map = M;

	revealed = false;
	seen = false;

	setRendered(false);

	usage = stringToUsage(desc->usage);

	manaCost = desc->manaCost;
	specTarget = 0;
	specType = stringToSpecType(desc->specific);

	equipped = false;

	stackable = desc->stackable;
	stacks = desc->stacks;
	destackCnt = 0;
	winsGame = desc->winsGame;
	
	morph::LookMulti *multi = new morph::LookMulti();
	multi->add(sprite = new morph::LookSprite(catchItemTexture(textureFileName), true), false);

	if (stackable) {
		stacksText = new morph::LookText(globalFontCont.getSfmlRes("rsrcs/fonts/arial.ttf"));
		stacksText->txt.setCharacterSize(stacksTextCharSize);
		makeStacksText();

		multi->add(stacksText, false);
	}

	setLook(multi);
}

void Item::makeStacksText() {
	if (!stackable || stacksText == 0) return;

	stacksText->txt.setString(std::to_string(stacks));
}

void Item::setMap(Map *M) {
	map = M;
}

void Item::missile(const Effect *E) {
	map->makeMissile(((EffectMissile*)E)->getSource(), this, ((EffectMissile*)E)->getMissile());
}

int Item::getMagicStat() const {
	Player *p = map->getPlayer();

	if (p) return p->getMagicStat();
	else return 0;
}

void Item::setRevealed(bool r) {
	if (revealed == r) return;

	revealed = r;

	if (revealed) {
		seen = true;
		setRendered(true);
		sprite->sprite.setColor(sf::Color(255, 255, 255));
	} else {
		sprite->sprite.setColor(sf::Color(255/2, 255/2, 255/2));
	}
}

bool Item::setSpecTarget(Tile *tile) {
	if (specType == S_ENEMY) {
		if (tile->hasEnemy()) {
			specTarget = tile->getEnemy();
			return true;
		} else {
			return false;
		}
	} else if (specType == S_CHARACTER) {
		if (map->tileHasPlayer(tile)) {
			specTarget = map->getPlayer();
			return true;
		} else if (tile->hasEnemy()) {
			specTarget = tile->getEnemy();
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

bool Item::hasEquipSlot(const std::string &slot) {
	for (unsigned i = 0; i < equipSlots.size(); ++i)
		for (unsigned j = 0; j < equipSlots[i].size(); ++j)
			if (equipSlots[i][j] == slot)
				return true;

	return false;
}

bool Item::needsSlots() const {
	return !equipSlots.empty();
}

bool Item::canEquip(Inventory *inv) {
	if (usage != U_EQUIPABLE) return false;

	if (!needsSlots()) return true;

	// program checks if there is a combination in which all these items are equipped

	// gather relevant items
	std::vector<Item*> items;
	items.push_back(this);

	for (unsigned i = 0; i < inv->slots.size(); ++i) {
		if (!inv->slots[i]->isEmpty() 
			&& inv->slots[i]->getItem() != this 
			&& inv->slots[i]->getItem()->isEquipped()
			&& inv->slots[i]->getItem()->needsSlots()) {
			items.push_back(inv->slots[i]->getItem());
		}
	}

	unsigned itemIndex = 0;

	// index of equip configuration and slot inside that configuration
	std::vector<std::pair<unsigned, unsigned>> slotIndex;
	slotIndex.push_back(std::pair<unsigned, unsigned>(0, 0));

	// occupied slots
	std::vector<std::string> takenSlots;

	while (true) {
		if (itemIndex >= items.size()) {
			// all items check out
			return true;
		}

		std::pair<unsigned, unsigned> currIndex = slotIndex.back();
		slotIndex.pop_back();

		if (currIndex.first >= items[itemIndex]->equipSlots.size()) {
			// all configurations have been tried

			// go back to previous item
			if (itemIndex > 0) {
				--itemIndex;

				unsigned prevConfIndexOfPrevItem = slotIndex.back().first - 1;

				for (unsigned i = 0; i < items[itemIndex]->equipSlots[prevConfIndexOfPrevItem].size(); ++i)
					takenSlots.pop_back();

				continue;
			} else {
				// no combination fits
				return false;
			}
		}

		if (currIndex.second >= items[itemIndex]->equipSlots[currIndex.first].size()) {
			// this configuration fits in with previously taken slots
			for (unsigned i = 0; i < items[itemIndex]->equipSlots[currIndex.first].size(); ++i)
				takenSlots.push_back(items[itemIndex]->equipSlots[currIndex.first][i]);

			// return index of next configuration/slot
			++currIndex.first;
			currIndex.second = 0;
			slotIndex.push_back(currIndex);

			// prepare to test the next item
			++itemIndex;
			slotIndex.push_back(std::pair<unsigned, unsigned>(0, 0));
			continue;
		}

		std::string currSlot = items[itemIndex]->equipSlots[currIndex.first][currIndex.second];

		if (std::find(takenSlots.begin(), takenSlots.end(), currSlot) == takenSlots.end()) {
			// this slot wasn't yet taken
			// prepare for next slot this item wants in current configuration
			++currIndex.second;
			slotIndex.push_back(currIndex);
		} else {
			// the slot was already occupied by a previous item
			// prepare to try the next configuration
			++currIndex.first;
			currIndex.second = 0;
			slotIndex.push_back(currIndex);
		}
	}
}

void Item::setStackSize(unsigned s) {
	if (stackable) {
		stacks = s;
		makeStacksText();
	}
}

void Item::incStackSize(unsigned n) {
	if (stackable) {
		stacks += n;
		makeStacksText();
	}
}

void Item::decStackSize() {
	if (stackable) {
		--stacks;
		makeStacksText();
	}
}

bool Item::incDestackCnt(unsigned n) {
	if (!stackable) {
		destackCnt = 1;
		return true;
	} else {
		if (stacks < destackCnt + n) return false;

		destackCnt += n;
		return true;
	}
}

void Item::resetDestackCnt() {
	destackCnt = 0;
}

void Item::destack() {
	if (!stackable) {
		if (destackCnt > 0) {
			removeItem();
		}
	} else {
		if (stacks <= destackCnt) {
			removeItem();
		} else {
			stacks -= destackCnt;
			resetDestackCnt();
			makeStacksText();
		}
	}
}

void Item::onPickup(Player *player) {
	if (winsGame) {
		map->winGame();
	}
}

void Item::onDrop(Player *player) {
}

std::string Item::toString() const {
	std::string txt = name + '\n';

	if (isRanged()) {
		txt = txt + "Range: " + std::to_string(getStat(S_RANGE)) + "\n";
	}

	for (unsigned i = 0; i < getStatsNum(); ++i) {
		StatVal statVal = stats[i];
		if (statVal.first == S_RANGE) continue;

		if (statVal.second != 0) {
			if (statVal.second > 0) txt = txt + "+";
			txt = txt + std::to_string(statVal.second) + " " + statToString(statVal.first) + "\n";
		}
	}

	if (isStackable()) {
		txt = txt + "Stacks: " + std::to_string(stacks) + "\n";
	}

	if (winsGame) {
		txt = txt + "Congratulations!\nPickup to win.";
	}

	return txt;
}

bool Item::putOutline(const sf::Vector2f &offset) {
	if (!hasOutline()) {
		morph::LookSprite *out = new morph::LookSprite(catchUITexture("purple_outline.png"), true);
		out->setPosition(offset);
		((morph::LookMulti*)getLook())->add(out, false);

		outlined = true;
		return true;
	} else {
		return false;
	}
}

bool Item::removeOutline() {
	if (hasOutline()) {
		((morph::LookMulti*)getLook())->popLast();

		outlined = false;
		return true;
	} else {
		return false;
	}
}

void Item::setCurrentTarget(const std::string &target) {
	if (target == "specific") currTarget = specTarget;
	else if (target == "player") currTarget = map->getPlayer();
	else if (target == "item") currTarget = this;
	else currTarget = this;
}

sf::Vector2f Item::getCenter() const {
	return getPosition() 
		+ sf::Vector2f(sprite->sprite.getGlobalBounds().width / 2, 
		sprite->sprite.getGlobalBounds().height / 2);
}

void Item::removeItem() {
	if (invSlot) invSlot->dropItem();
	getRoom()->destroyObject(this);
}

void Item::destroy(const Effect *E) {
	onTrigger(Trigger::T_DESTROY);

	if (!stackable || stacks <= 1) {
		removeItem();
	} else {
		decStackSize();
	}
}