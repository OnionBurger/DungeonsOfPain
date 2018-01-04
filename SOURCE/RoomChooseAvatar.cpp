#include "RoomChooseAvatar.h"
#include "Descriptors.h"
#include "RoomDungeon.h"
#include "RoomMain.h"
#include "GlobalTextureCont.h"
#include "GlobalFontCont.h"
#include "DescriptorAvatar.h"

RoomChooseAvatar::RoomChooseAvatar() {
	desc = new Descriptors();
}

void RoomChooseAvatar::onEnter() {
	avatarIndex = 0;
	desc->read();

	avatarLook.setPosition(96, 96);

	avatarName.setCharacterSize(16);
	avatarName.setFont(globalFontCont.getSfmlRes("rsrcs/fonts/arial.ttf"));
	avatarName.setPosition(96 + 2 * 32, 96);

	avatarDesc.setCharacterSize(16);
	avatarDesc.setFont(globalFontCont.getSfmlRes("rsrcs/fonts/arial.ttf"));
	avatarDesc.setPosition(96 + 2 * 32, 96 + 32);

	updateAvatarShowcase();
}

void RoomChooseAvatar::updateAvatarShowcase() {
	avatarLook.setTexture(catchAvatarTexture(desc->getDescAvatar(avatarIndex)->textureFileName), true);
	avatarName.setString(desc->getDescAvatar(avatarIndex)->name);
	avatarDesc.setString(desc->getDescAvatar(avatarIndex)->description);
}

void RoomChooseAvatar::prevAvatar() {
	if (avatarIndex == 0) avatarIndex = desc->getDescAvatarSize() - 1;
	else --avatarIndex;

	updateAvatarShowcase();
}

void RoomChooseAvatar::nextAvatar() {
	if (avatarIndex + 1 >= desc->getDescAvatarSize()) avatarIndex = 0;
	else ++avatarIndex;

	updateAvatarShowcase();
}

void RoomChooseAvatar::postEvent(const sf::Event &e) {
	if (e.type == sf::Event::KeyPressed) {
		if (e.key.code == sf::Keyboard::Escape) {
			delete desc;
			changeRoom(new RoomMain());
		} else if (e.key.code == sf::Keyboard::Return || e.key.code == sf::Keyboard::Space || 
			e.key.code == sf::Keyboard::LControl || e.key.code == sf::Keyboard::RControl) {
			changeRoom(new RoomDungeon(0, avatarIndex, desc));
		} else if (e.key.code == sf::Keyboard::Left || e.key.code == sf::Keyboard::Down || 
			e.key.code == sf::Keyboard::A || e.key.code == sf::Keyboard::S) {
			prevAvatar();
		} else if (e.key.code == sf::Keyboard::Right || e.key.code == sf::Keyboard::Up || 
			e.key.code == sf::Keyboard::D || e.key.code == sf::Keyboard::W) {
			nextAvatar();
		}
	}
}

void RoomChooseAvatar::drawSelf(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(avatarLook, states);
	target.draw(avatarName, states);
	target.draw(avatarDesc, states);
}