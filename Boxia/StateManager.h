#ifndef STATEMANAGER_H
#define STATEMANAGER_H


class ApplicationState;
class GameState;
class MenuState;
class ZD3DApp;

class StateManager
{
public:
  StateManager(ZD3DApp* application);
  void Initialize();

  void Update(double delta);
  void Draw();

private:
  ZD3DApp* application;
  ApplicationState* currentState;
  GameState* gameState;
  MenuState* menuState;
};

#endif