#pragma once

namespace pla::games {

enum class States
{
  GameChoosing,
  Lobby,
  Game
};

class IState
{
public:
  ~IState() = default;

  /**
   * Initialize method for given state.
   */
  virtual void init() = 0;

  /**
   * Event handling method.
   */
  virtual void eventHandling() = 0;

  /**
   * Display method.
   */
  virtual void display() = 0;

protected:
  IState() = default;
};

} // namespace
