#pragma once

#include <SFML/Graphics.hpp>

#include <map>

#include "../../ResourcePath.hpp"

//DEBUG
#include <iostream>
using namespace std;

namespace Resources {
    enum Textures {
        PLAYER_MOVEMENT, PLAYER_IDLE,
        TESTLEVEL_BACKGROUND, TESTLEVEL_COLLISION, TESTLEVEL_FOREGROUND,
        TEST_ENTITY
    };

    class ResourceHolder{
    public:
        void Load(const enum Textures& texturesEnum, const std::string &filename);
        sf::Texture &Get(enum Textures texturesEnum);
        const sf::Texture &Get(enum Textures texturesEnum) const;
    private:
        std::map<enum Textures, std::unique_ptr<sf::Texture>> _mResourcesMap;
    };
}
