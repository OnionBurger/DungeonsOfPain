#ifndef _Item_h_
#define _Item_h_

#include <Morph\Object.h>
#include "WithTriggers.h"
#include "WithStats.h"
#include "Target.h"
#include <string>
#include <Morph\Looks\LookText.h>
#include <Morph\Looks\LookSprite.h>
#include <Morph\Looks\LookMulti.h>

class Player;
class DescriptorItem;
class Map;
class InventorySlot;
class Inventory;
class Tile;

class Item : public morph::Object, public WithStats, public WithTriggers, public Target {
public:
	enum Usage {
		U_UNKNOWN, 
		U_NONE, 
		U_EQUIPABLE, 
		U_USABLE
	};

	static Usage stringToUsage(const std::string &str);

	enum SpecType {
		S_NONE, 
		S_ENEMY, 
		S_CHARACTER
	};

	static SpecType stringToSpecType(const std::string &str);

private:
	friend class Player;

	DescriptorItem *desc;

	Map *map;
	InventorySlot *invSlot;

	Usage usage;

	std::string handle, name;
	std::string textureFileName;
	morph::LookSprite *sprite;

	bool stackable;
	unsigned stacks;
	unsigned destackCnt;
	morph::LookText *stacksText;

	void makeStacksText();

	bool revealed;
	bool seen;

	bool equipped;
	std::vector<std::vector<std::string>> equipSlots;
	std::string ammoSlot;

	int manaCost;
	Target *specTarget;
	SpecType specType;

	bool winsGame;

	void setCurrentTarget(const std::string &target);

	sf::Vector2f getCenter() const;

	void removeItem();
	void destroy(const Effect *E);
	
	bool outlined;

public:
	Item(DescriptorItem *desc, Map *M, StorageEffects *S);

	DescriptorItem* getDescriptor() { return desc; }

	const std::string& getHandle() const { return handle; }

	void setMap(Map *M);

	void setInventorySlot(InventorySlot *S) { invSlot = S; }
	
	void missile(const Effect *E);
	
	virtual int getMagicStat() const;

	void setRevealed(bool r);
	bool isRevealed() const { return revealed; }
	bool isKnown() const { return seen; }

	Usage getUsage() const { return usage; }

	int getManaCost() const { return manaCost; }

	SpecType getSpecType() const { return specType; }
	bool setSpecTarget(Tile *tile);

	bool isEquipped() const { return equipped; }
	void setEquipped(bool b) { equipped = b; }

	bool hasEquipSlot(const std::string &slot);

	bool needsSlots() const;
	bool canEquip(Inventory *inv);

	bool isStackable() const { return stackable; }
	unsigned getStackSize() const { return stacks; }
	void setStackSize(unsigned s);

	void incStackSize(unsigned n);
	void decStackSize();

	unsigned getDestackCnt() const { return destackCnt; }
	bool incDestackCnt(unsigned n = 1);
	void resetDestackCnt();
	void destack();

	bool getWinsGame() const { return winsGame; }
	void setWinsGame(bool b) { winsGame = b; }

	void onPickup(Player *player);
	void onDrop(Player *player);

	bool hasOutline() const { return outlined; }
	bool waitingTarget() const { return hasOutline(); }
	bool putOutline(const sf::Vector2f &offset);
	bool removeOutline();

	std::string toString() const;
};

#endif