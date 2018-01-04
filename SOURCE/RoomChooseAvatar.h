#ifndef _Room_Choose_Avatar_h_
#define _Room_Choose_Avatar_h_

#include <Morph\Room.h>

class Descriptors;

class RoomChooseAvatar : public morph::Room {
	unsigned avatarIndex;

	Descriptors *desc;

	sf::Sprite avatarLook;
	sf::Text avatarName, avatarDesc;

	void updateAvatarShowcase();

	void prevAvatar();
	void nextAvatar();

public:
	RoomChooseAvatar();
	
	void onEnter();

	void postEvent(const sf::Event &e);
	void drawSelf(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif