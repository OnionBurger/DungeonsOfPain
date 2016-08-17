#ifndef _Combatant_h_
#define _Combatant_h_

#include "Target.h"
#include "WithStats.h"
#include "WithTriggers.h"
#include "BuffHolder.h"
#include <string>

class DescriptorCombatant;
class Map;

class Combatant : public Target, public WithStats, public WithTriggers, public BuffHolder {
protected:
	Map *map;
	unsigned tileX, tileY;

	int currHP, currMP;

	Combatant *other;

	virtual int getRange() const;

	void changeMaxHP(int offset);
	void changeCurrHP(int offset);
	
	void changeMaxMP(int offset);
	void changeCurrMP(int offset);

	virtual int getDmg() const;
	virtual float getDodgeChance() const;
	virtual float getCritChance() const;
	virtual float getCritMultiplier() const;
	virtual float getAmmoRetrChance() const;
	virtual float getMagicDmgIgnore() const;

public:
	Combatant(const DescriptorCombatant *desc, StorageEffects *S);

	void setLoc(Map *M, unsigned X, unsigned Y);
	void setLoc(unsigned X, unsigned Y);
	unsigned getLocX() const { return tileX; }
	unsigned getLocY() const { return tileY; }

	virtual int getStat(Stat S) const;
	virtual bool isRanged() const;
	virtual std::string getMissileTextureFileName() const;

	virtual int getMagicStat() const { return getStat(S_INT); }

	virtual bool isImmobile() const;

	bool isDead() const { return currHP <= 0; }
	virtual void onDeath() =0;

	int coordDistance(unsigned destX, unsigned destY) const;
	bool inRangeForAttack(unsigned destX, unsigned destY) const;

	bool shouldCrit() const;
	int calcCombatDmgNormal() const;
	int calcCombatDmgCrit() const;

	void getAttacked(Combatant *attacker);

	virtual sf::Vector2f getCenter() const;
	
	virtual bool onTrigger(Trigger::Trigg tr);
	void setCurrentTarget(const std::string &target);

	virtual void destroy(const Effect *E);
	virtual void heal(const Effect *E);
	virtual void mana(const Effect *E);
	virtual void hurt(const Effect *E);
	virtual void missile(const Effect *E);
	virtual void buff(const Effect *E);

	virtual Target* getSelf();
	virtual Target* getOther();
	virtual Target* getPlayer();

	virtual std::string combatStatsToString() const;

	virtual ~Combatant() {}
};

#endif