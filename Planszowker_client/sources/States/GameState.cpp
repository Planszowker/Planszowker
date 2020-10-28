#include "GameState.hpp"

namespace eng{
    GameState::GameState(){
        _mScene.AddSceneNode(1, SceneNodeTypes::PLAYER);
        
        _mScene.InitializeSceneNodes();
        _mScene.InitializeWorld();
        
        _mView.setSize(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f);
    }
    
    void GameState::Clear(){
        _mScene.ClearScene(sf::Color::White);
    }
    
    void GameState::HandleEvent(){
        _mScene.HandleSceneNodesEvents();
    }
    
    void GameState::HandleInput(){
        _mScene.HandleSceneNodesInputs();
    }
    
    void GameState::Update(){
        _mView.setCenter(_mScene.GetSceneNode(1)->GetPosition().x, _mScene.GetSceneNode(1)->GetPosition().y);
        Window::SetView(_mView);
        
        _mScene.UpdateScene();
        
        _mScene.WorldLoop();
    }
    
    void GameState::Render(){
        _mScene.DisplayScene();
    }
}
