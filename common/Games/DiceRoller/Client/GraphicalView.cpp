#include "GraphicalView.h"

#include "DiceRoller/Version.h"
#include "DiceRoller/Objects.h"

#include <iostream>
#include <chrono>
#include <string>
#include <atomic>

using namespace pla::common::games;

namespace pla::common::games::dice_roller {

DiceRollerGraphicalView::DiceRollerGraphicalView(const sf::Vector2i& windowDim, const std::string& windowName)
  : GraphicalView(windowDim, windowName)
  , m_buttonRollReroll(std::move(sfg::Button::Create()))
  , m_buttonConfirm(std::move(sfg::Button::Create()))
{
}


void DiceRollerGraphicalView::update(const std::any& objectFromController) {
  //auto receivedObject = std::any_cast<std::string>(object);

  //std::cout << receivedObject << std::endl;

  // TODO: Remove below printout...
  //std::cout << "Received additional info: " << receivedObject.additionalInfo << "\n";
  //std::cout << "Reply: " << static_cast<size_t>(receivedObject.reply) << "\n";
}


void DiceRollerGraphicalView::init() {
  // Set desktop properties
  m_desktop.SetProperties(
          "Button {"
          " FontSize: 26;"
          "}"
  );

  // Roll/Reroll button
  m_buttonRollReroll->SetLabel("Roll");
  m_buttonRollReroll->SetPosition(sf::Vector2f(200, 300));

  // Confirm button
  m_buttonConfirm->SetLabel("Confirm");
  m_buttonConfirm->SetPosition(sf::Vector2f(400, 300));

  m_widgets.push_back(m_buttonRollReroll);
  m_widgets.push_back(m_buttonConfirm);

  // Add all widgets to desktop
  for (const auto& widget : m_widgets)
  {
    m_desktop.Add(widget);
  }

  // Some basic info about a game
  std::cout << "DiceRoller (Graphical) v" << DiceRollerVersionMajor << "." << DiceRollerVersionMinor << "." << DiceRollerVersionPatch << "\n";
  std::cout << "========================================\n";
  std::cout << "Choose an action. You can reroll up to 1 time. Points are summed. Player with a greater score wins.\n";
  std::cout << "========================================\n";
}


void DiceRollerGraphicalView::_eventHandler(sf::Event& event)
{
}


void DiceRollerGraphicalView::_display()
{
  //m_gameWindow->clear(sf::Color::Magenta);

  sf::RectangleShape rect(sf::Vector2f(250.f, 80.f));
  rect.setFillColor(sf::Color::Blue);

  m_gameWindow->setView(m_gameView);
  m_gameWindow->draw(rect);

  m_gameWindow->setView(m_playersView);
  m_gameWindow->draw(rect);
}

} // namespaces
