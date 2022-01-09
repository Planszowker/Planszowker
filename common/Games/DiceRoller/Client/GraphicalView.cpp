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
  , m_rollRerollButton(sfg::Button::Create("Roll"))
  , m_confirmButton(sfg::Button::Create("Confirm"))
{
  m_rollRerollButton->SetRequisition(sf::Vector2f(MinimalButtonWidth, MinimalButtonHeight));
  m_confirmButton->SetRequisition(sf::Vector2f(MinimalButtonWidth * 3.f, MinimalButtonHeight));

  m_textureLoader.loadAndStoreTexture("data/common/dice/DiceSprSheetX128.png", "dice");
}


void DiceRollerGraphicalView::update(const std::any& objectFromController) {
  //auto receivedObject = std::any_cast<std::string>(object);

  //std::cout << receivedObject << std::endl;

  // TODO: Remove below printout...
  //std::cout << "Received additional info: " << receivedObject.additionalInfo << "\n";
  //std::cout << "Reply: " << static_cast<size_t>(receivedObject.reply) << "\n";
}


void DiceRollerGraphicalView::init() {
  // Some basic info about a game
  std::cout << "DiceRoller (Graphical) v" << DiceRollerVersionMajor << "." << DiceRollerVersionMinor << "." << DiceRollerVersionPatch << "\n";
  std::cout << "========================================\n";
  std::cout << "Choose an action. You can reroll up to 1 time. Points are summed. Player with a greater score wins.\n";
  std::cout << "========================================\n";

  m_actionAreaBox->SetOrientation(sfg::Box::Orientation::HORIZONTAL);
  m_actionAreaBox->Pack(m_rollRerollButton, false);
  m_actionAreaBox->Pack(m_confirmButton, false);

  std::cout << "Req: " << m_actionsAreaWindow->GetRequisition().x << " " << m_actionsAreaWindow->GetRequisition().y << "\n";

  m_rollRerollButton->GetSignal(sfg::Button::OnMouseLeftRelease).Connect([this]{
    std::cout << "Roll/Reroll button pressed.\n";
    this->m_rollRerollButton->Show(false);
    this->m_confirmButton->Show(true);
    this->m_actionAreaBox->Pack(this->m_confirmButton);
    this->m_actionAreaBox->Remove(this->m_rollRerollButton);
  });

  m_confirmButton->GetSignal(sfg::Button::OnMouseLeftRelease).Connect([this]{
    std::cout << "Confirm button pressed.\n";
    this->m_confirmButton->Show(false);
    this->m_rollRerollButton->Show(true);
    this->m_actionAreaBox->Pack(this->m_rollRerollButton);
    this->m_actionAreaBox->Remove(this->m_confirmButton);
  });
}


void DiceRollerGraphicalView::_eventHandler(sf::Event& event)
{

}


void DiceRollerGraphicalView::_display()
{
  const auto& [found, diceTexture] = m_textureLoader.retrieveTexture("dice");

  sf::Sprite ziuziu;
  if (found) {
    ziuziu.setTexture(*diceTexture);
  }

  m_gameAreaSfmlCanvas->Draw(ziuziu);
}

} // namespaces
