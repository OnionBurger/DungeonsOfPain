#ifndef _Buff_h_
#define _Buff_h_

#include "WithTriggers.h"
#include "WithStats.h"
#include "DescriptorBuff.h"

class StorageEffects;
class BuffHolder;

class Buff : public WithStats, public WithTriggers {
public:
	struct Duration {
		Trigger::Trigg type;
		unsigned startVal;

	private:
		unsigned currVal;

	public:
		explicit Duration(const DescriptorBuff::Duration &d);

		void decrease() { if (currVal > 0) --currVal; }

		bool isEndless() const { return startVal == 0; }
		bool isOver() const { return !isEndless() && currVal == 0; }
	};

private:
	BuffHolder *holder;

	std::string handle, name;

	std::vector<Duration> durations;

	bool over;

public:
	Buff(const DescriptorBuff *descBuff, StorageEffects *S);

	void setHolder(BuffHolder *h) { holder = h; }

	bool onTrigger(Trigger::Trigg tr);
	void setCurrentTarget(const std::string &target);

	bool isOver() const { return over; }
};

#endif