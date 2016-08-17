#include "EffectSound.h"
#include "GlobalSoundBufferCont.h"

EffectSound::EffectSound(const pugi::xml_node &node) : Effect(node) {
	soundFileName = node.attribute("file").as_string("");
}

void EffectSound::run(Target *T) {
	sound.setBuffer(catchSound(soundFileName));
	sound.play();
}