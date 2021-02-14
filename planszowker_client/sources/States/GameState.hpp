#pragma once

#include "../Util/Window.hpp"

#include "IStates.h"

#include "../Entities/Player.hpp"
#include "../../Scene.hpp"

class Core;

namespace eng {
    class GameState :public StatesBase{
    public:
        GameState();
        
        void Clear();
        void HandleEvent();
        void HandleInput();
        void Update();
        void Render();
    private:
        Scene _mScene;
        sf::View _mView;
    };
}

