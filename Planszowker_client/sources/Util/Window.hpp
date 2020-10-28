#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

#define WINDOW_WIDTH 1080
#define WINDOW_HEIGHT 720

namespace eng {
    const float PPM = 30.f;
    
    class Window{
    public:
        static sf::RenderWindow& GetWindow();
        
        static void InitWindow();
        
        static void ClearWindow(sf::Color color = sf::Color::Black);
        
        static void HandleEvents();
        
        static void DisplayWindow();
        
        static bool IsWindowOpen();
        
        static void CloseWindow();
        
        static void SetView(const sf::View& view);
    private:
        Window() { }
        static sf::RenderWindow _mWindow;
    };
}
