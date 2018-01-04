#include "BuffHolder.h"

BuffHolder::BuffHolder() {
}

void BuffHolder::addBuff(Buff *B) {
	if (B && !B->isOver()) {
		buffs.push_back(B);
		B->setHolder(this);
	}
}

int BuffHolder::getStat(WithStats::Stat S) const {
	if (S == WithStats::S_RANGE) return getRange();

	int ret = 0;

	for (std::list<Buff*>::const_iterator it = buffs.cbegin(); it != buffs.cend(); ++it) {
		ret += (*it)->getStat(S);
	}

	return ret;
}

bool BuffHolder::isRanged() const {
	for (std::list<Buff*>::const_iterator it = buffs.cbegin(); it != buffs.cend(); ++it) {
		if ((*it)->isRanged()) return true;
	}

	return false;
}

int BuffHolder::getRange() const {
	int ret = 0;

	for (std::list<Buff*>::const_iterator it = buffs.cbegin(); it != buffs.cend(); ++it) {
		if (ret < (*it)->getStat(WithStats::S_RANGE)) ret = (*it)->getStat(WithStats::S_RANGE);
	}

	return ret;
}

std::string BuffHolder::getMissileTextureFileName() const {
	for (std::list<Buff*>::const_iterator it = buffs.cbegin(); it != buffs.cend(); ++it) {
		if ((*it)->isRanged() && !((*it)->getMissileTextureFileName().empty())) {
			return (*it)->getMissileTextureFileName();
		}
	}

	return std::string();
}

bool BuffHolder::isImmobile() const {
	for (std::list<Buff*>::const_iterator it = buffs.cbegin(); it != buffs.cend(); ++it) {
		if ((*it)->isImmobile())
			return true;
	}

	return false;
}

bool BuffHolder::onTrigger(Trigger::Trigg tr) {
	bool ret = false;

	for (std::list<Buff*>::iterator it = buffs.begin(); it != buffs.end();) {
		bool eraseCalled = false;

		if ((*it)->onTrigger(tr)) {
			ret = true;

			if ((*it)->isOver()) {
				delete *it;

				it = buffs.erase(it); // it now points to the next element in the list
				eraseCalled = true;
			}
		}

		if (!eraseCalled) ++it;
	}

	return ret;
}

BuffHolder::~BuffHolder() {
	for (std::list<Buff*>::iterator it = buffs.begin(); it != buffs.end(); ++it)
		delete *it;
}