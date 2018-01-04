#ifndef _Global_Sound_Buffer_Cont_h_
#define _Global_Sound_Buffer_Cont_h_

#include <Morph\ResConts\SoundBufferCont.h>
#include <string>

extern morph::SoundBufferContainer globalSoundBufferCont;

const sf::SoundBuffer& catchSound(const std::string &fileName);

#endif