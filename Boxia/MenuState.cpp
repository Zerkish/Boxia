#include "MenuState.h"
#include <ZGraphics.h>
#include <ZD3DApp.h>
#include <ZKeyboard.h>

MenuState::MenuState(ZD3DApp* application)
  :ApplicationState(application)
{
  SetNextState(Menu);
}

void MenuState::Initialize()
{
  graphics = new ZGraphics(GetApplication()->GetGraphicsDevice());
}

void MenuState::Activate()
{
  SetNextState(Menu);
}

void MenuState::Deactivate()
{

}



void MenuState::Update(double delta)
{
  if(ZKeyboard::IsKeyDown(Keys::Esc))
  {
    SetNextState(Quit);
  }
  if(ZKeyboard::IsKeyDown(Keys::P))
  {
    SetNextState(Game);
  }
  
}

void MenuState::Draw()
{

}