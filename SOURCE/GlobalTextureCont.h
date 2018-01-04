#ifndef _Global_Texture_Cont_h_
#define _Global_Texture_Cont_h_

#include <Morph\ResConts\TextureCont.h>
#include <string>

extern morph::TextureContainer globalTextureCont;

const sf::Texture& catchAvatarTexture(const std::string &fileName);
const sf::Texture& catchEnemyTexture(const std::string &fileName);
const sf::Texture& catchItemTexture(const std::string &fileName);
const sf::Texture& catchWallTexture(const std::string &fileName);
const sf::Texture& catchDoorTexture(const std::string &fileName);
const sf::Texture& catchFloorTexture(const std::string &fileName);
const sf::Texture& catchStairsTexture(const std::string &fileName);
const sf::Texture& catchFxTexture(const std::string &fileName);
const sf::Texture& catchMenuTexture(const std::string &fileName);
const sf::Texture& catchUITexture(const std::string &fileName);

#endif