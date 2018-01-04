#ifndef _RoomDungeon_h_
#define _RoomDungeon_h_

#include <Morph\Room.h>

class Descriptors;

class RoomDungeon : public morph::Room {
	unsigned lvlNum, avatarInd;

	bool isDescOwner;
	Descriptors *desc;

public:
	RoomDungeon(unsigned levelNumber, unsigned avatarIndex, Descriptors *D);

	void onEnter();

	unsigned getNumberOfLevels() const;
	unsigned getLevelNumber() const { return lvlNum; }
	bool isLastLevel() const { return lvlNum + 1 >= getNumberOfLevels(); }

	const std::string& getLookDoor() const;
	const std::string& getLookFloor() const;
	const std::string& getLookWall() const;

	bool goToNextLevel();

	~RoomDungeon();
};

#endif