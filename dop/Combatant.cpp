#include "Combatant.h"
#include "getRand.h"
#include "EffectHeal.h"
#include "EffectMana.h"
#include "EffectHurt.h"
#include "EffectMissile.h"
#include "EffectBuff.h"
#include "Map.h"
#include "DescriptorCombatant.h"
#include "Missile.h"
#include "Player.h"
#include "Descriptors.h"
#include "DescriptorBuff.h"

Combatant::Combatant(const DescriptorCombatant *desc, StorageEffects *S) : 
	WithStats(desc), WithTriggers(desc, S) {
	map = 0;
	tileX = 0;
	tileY = 0;

	currHP = getStat(S_MAX_HP);
	currMP = getStat(S_MAX_MP);

	other = 0;
}

void Combatant::setLoc(Map *M, unsigned X, unsigned Y) {
	map = M;
	setLoc(X, Y);
}

void Combatant::setLoc(unsigned X, unsigned Y) {
	tileX = X;
	tileY = Y;
}

int Combatant::getRange() const {
	unsigned range1 = WithStats::getStat(S_RANGE);
	unsigned range2 = BuffHolder::getStat(S_RANGE);

	return range1 > range2 ? range1 : range2;
}

int Combatant::getStat(Stat S) const {
	if (S == WithStats::S_RANGE) {
		return getRange();
	}

	return WithStats::getStat(S) + BuffHolder::getStat(S);
}

bool Combatant::isRanged() const {
	return WithStats::isRanged() || BuffHolder::isRanged();
}

std::string Combatant::getMissileTextureFileName() const {
	std::string missile1(BuffHolder::getMissileTextureFileName());
	if (!missile1.empty()) return missile1;

	return WithStats::getMissileTextureFileName();
}

bool Combatant::isImmobile() const {
	return WithStats::isImmobile() || BuffHolder::isImmobile();
}

int Combatant::coordDistance(unsigned destX, unsigned destY) const {
	unsigned diffX, diffY;
	diffX = tileX >= destX ? tileX - destX : destX - tileX;
	diffY = tileY >= destY ? tileY - destY : destY - tileY;

	return diffX + diffY;
}

bool Combatant::inRangeForAttack(unsigned destX, unsigned destY) const {
	if (tileX == destX && tileY == destY) return true;

	if (!isRanged()) {
		if (tileX == destX) {
			return tileY + 1 == destY || destY + 1 == tileY;
		}
		else if (tileY == destY) {
			return tileX + 1 == destX || destX + 1 == tileX;
		}
		else {
			return false;
		}
	} else {
		return coordDistance(destX, destY) <= getRange();
	}
}

bool Combatant::shouldCrit() const {
	return getRandFloat(100) < getCritChance();
}

int Combatant::calcCombatDmgNormal() const {
	return getDmg();
}

int Combatant::calcCombatDmgCrit() const {
	return (int)(getCritMultiplier() * getDmg());
}

void Combatant::getAttacked(Combatant *attacker) {
	if (attacker == 0) return;

	other = attacker;
	attacker->other = this;

	if (getRandFloat(100) < getDodgeChance()) {
		onTrigger(Trigger::T_AVOID);
		attacker->onTrigger(Trigger::T_MISS);

		return;
	}

	bool shouldCrit = attacker->shouldCrit();
	int dmgTaken = shouldCrit ? attacker->calcCombatDmgCrit() : attacker->calcCombatDmgNormal();
	if (dmgTaken <= 0) return;

	if (getStat(S_P_RES) > 0) dmgTaken -= getStat(S_P_RES);
	if (dmgTaken <= 0) dmgTaken = 1;

	currHP -= dmgTaken;

	onTrigger(Trigger::T_STRUCK);
	attacker->onTrigger(Trigger::T_HIT);
	if (shouldCrit) attacker->onTrigger(Trigger::T_CRIT);
}

sf::Vector2f Combatant::getCenter() const {
	return sf::Vector2f(getLocX() * map->getTileSize().x + map->getTileSize().x / 2, 
		getLocY() * map->getTileSize().y + map->getTileSize().y / 2);
}

bool Combatant::onTrigger(Trigger::Trigg tr) {
	bool ret1 = WithTriggers::onTrigger(tr);
	bool ret2 = BuffHolder::onTrigger(tr);

	return ret1 || ret2;
}

void Combatant::setCurrentTarget(const std::string &target) {
	if (target == "other") currTarget = other;
	else if (target == "self") currTarget = this;
	else if (target == "player") currTarget = map->getPlayer();
	else currTarget = this;
}

void Combatant::destroy(const Effect *E) {
	onTrigger(Trigger::T_DESTROY);
	onDeath();
}

void Combatant::heal(const Effect *E) {
	changeCurrHP(((const EffectHeal*)E)->getAmount());
}

void Combatant::mana(const Effect *E) {
	changeCurrMP(((const EffectMana*)E)->getAmount());
}

void Combatant::hurt(const Effect *E) {
	// 0.5 is added to round to the closest integer
	int dmg = (int)(((const EffectHurt*)E)->getAmount() * (1.f - getMagicDmgIgnore()) + 0.5f);
	if (dmg > 0) changeCurrHP(dmg * -1);
}

void Combatant::missile(const Effect *E) {
	map->makeMissile(((EffectMissile*)E)->getSource(), this, ((EffectMissile*)E)->getMissile());
}

void Combatant::buff(const Effect *E) {
	addBuff(new Buff(map->getDescriptors()->getDescBuff(((EffectBuff*)E)->getBuffHandle()), 
		map->getDescriptors()->getEffects()));
}

Target* Combatant::getSelf() {
	return this;
}

Target* Combatant::getOther() {
	return other;
}

Target* Combatant::getPlayer() {
	return map->getPlayer();
}

std::string Combatant::combatStatsToString() const {
	std::string txt = "Health: " + std::to_string(currHP) + "/" + std::to_string(getStat(S_MAX_HP));

	if (getStat(S_MAX_MP) > 0) {
		txt = txt + "\nMana: " + std::to_string(currMP) + "/" + std::to_string(getStat(S_MAX_MP));
	}

	if (isRanged())
		txt = txt + "\n" + "Range: " + std::to_string(getRange());

	for (unsigned i = 0; i < getStatsNum(); ++i) {
		Stat stat = getStatByIndex(i);
		int statVal = getStatValByIndex(i);
		if (stat == S_MAX_HP || stat == S_RANGE) continue;

		txt = txt + "\n" + statToString(stat) + ": " + std::to_string(statVal);
	}

	return txt;
}

void Combatant::changeMaxHP(int offset) {
	setStat(S_MAX_HP, getStat(S_MAX_HP) + offset);
	if (currHP > getStat(S_MAX_HP)) currHP = getStat(S_MAX_HP);
}

void Combatant::changeCurrHP(int offset) {
	currHP += offset;
	if (currHP > getStat(S_MAX_HP)) currHP = getStat(S_MAX_HP);
	else if (isDead()) onDeath();
}

void Combatant::changeMaxMP(int offset) {
	setStat(S_MAX_MP, getStat(S_MAX_MP) + offset);
	if (currMP > getStat(S_MAX_MP)) currMP = getStat(S_MAX_MP);
}

void Combatant::changeCurrMP(int offset) {
	currMP += offset;
	if (currMP > getStat(S_MAX_MP)) currMP = getStat(S_MAX_MP);
	else if (currMP < 0) currMP = 0;
}

int Combatant::getDmg() const {
	return isRanged() ? getStat(S_ACC) : getStat(S_STR);
}

float Combatant::getDodgeChance() const {
	if (getStat(S_DXT) <= 0) return 0;

	float ret = 100.f * getStat(S_DXT) / (20.f + getStat(S_DXT));

	if (ret < 0) ret = 0;
	else if (ret > 100) ret = 100;

	return ret;
}

float Combatant::getCritChance() const {
	if (getStat(S_DED) <= 0) return 0;

	float ret = 100.f * getStat(S_DED) / (20.f + getStat(S_DED));

	if (ret < 0) ret = 0;
	else if (ret > 100) ret = 100;

	return ret;
}

float Combatant::getCritMultiplier() const {
	float base = 2.f;

	if (getStat(S_DED) <= 0) return base;

	float ret = base + 0.25f * (getStat(S_DED) / 5);

	if (ret < base) ret = base;

	return ret;
}

float Combatant::getAmmoRetrChance() const {
	if (getStat(S_ACC) <= 0) return 0;

	float ret = 100.f * getStat(S_ACC) / (7.f + getStat(S_ACC));

	if (ret < 0) ret = 0;
	else if (ret > 100) ret = 100;

	return ret;
}

float Combatant::getMagicDmgIgnore() const {
	if (getStat(S_M_RES) <= 0) return 0;

	float ret = getStat(S_M_RES) / 100.f;

	if (ret < 0) ret = 0;
	else if (ret > 1) ret = 1;

	return ret;
}