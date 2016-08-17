#include "GlobalSoundBufferCont.h"

morph::SoundBufferContainer globalSoundBufferCont;

const sf::SoundBuffer& catchSound(const std::string &fileName) {
	try {
		return globalSoundBufferCont.getSfmlRes("rsrcs/audio/sounds/" + fileName);
	} catch (morph::SoundBufferContainer::ErrNotFound e) {
		return globalSoundBufferCont.getSfmlRes("rsrcs/audio/sounds/unknown_sound.wav");
	}
}