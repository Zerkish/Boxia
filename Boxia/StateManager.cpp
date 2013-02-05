#include "ApplicationState.h"
#include "GameState.h"
#include "MenuState.h"
#include "StateManager.h"
#include <ZD3DApp.h>
#include <cassert>

StateManager::StateManager(ZD3DApp* application)
{
  this->application = application;
  currentState = nullptr;
  gameState = nullptr;
  menuState = nullptr;
}

void StateManager::Initialize()
{
  menuState = new MenuState(application);
  menuState->Initialize();
  menuState->Activate();
  currentState = menuState;
}

void StateManager::Update(double delta)
{
  if(!currentState) return;

  currentState->Update(delta);

  switch(currentState->GetNextState())
  {
  case ApplicationStates::Menu:
    if(currentState != menuState)
    {
      if(menuState == nullptr)
      {
        menuState = new MenuState(application);
        menuState->Initialize();
      }
      currentState->Deactivate();
      currentState = menuState;
      currentState->Activate();
    }
    return;
  case ApplicationStates::Game:
    if(currentState != gameState)
    {
      if(gameState == nullptr)
      {
        gameState = new GameState(application);
        gameState->Initialize();
      }
      currentState->Deactivate();
      currentState = gameState;
      currentState->Activate();
    }
    return;
  case ApplicationStates::Quit:
    application->Quit();
    return;
  default:
    assert(false);
  };
}

void StateManager::Draw()
{
  if(currentState) currentState->Draw();
}

