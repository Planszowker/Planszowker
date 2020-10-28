#include "Window.hpp"

namespace eng{
    sf::RenderWindow Window::_mWindow;
    
    void Window::InitWindow(){
        Window::_mWindow.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32), "Toborson RPG Engine v. 0.0.0 - NOT EVEN ALPHA");
        Window::_mWindow.setVerticalSyncEnabled(false);
    }
    
    sf::RenderWindow& Window::GetWindow(){
        return Window::_mWindow;
    }

    void Window::ClearWindow(sf::Color color){
        Window::_mWindow.clear(color);
    }

    void Window::DisplayWindow(){
        Window::_mWindow.display();
    }

    bool Window::IsWindowOpen(){
        return Window::_mWindow.isOpen();
    }

    void Window::HandleEvents(){
        sf::Event event;
        
        while(_mWindow.pollEvent(event)){
            switch(event.type){
                case sf::Event::Closed:
                    Window::_mWindow.close();
                    break;
                default:
                    break;
            }
        }
    }

    void Window::CloseWindow(){
        Window::_mWindow.close();
    }

    void Window::SetView(const sf::View& view){
        Window::_mWindow.setView(view);
    }

}
