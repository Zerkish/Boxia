#include <LibZerkish.h>

#include "MenuState.h"


#include <d3d10.h>
#include <dxgi.h>
#include <d3dx10.h>

MenuState::MenuState(ZD3DApp* application)
  :ApplicationState(application)
{
  SetNextState(Menu);
}

void MenuState::Initialize()
{
  //graphics = new ZGraphics();
  //graphics->InitializeDefault(GetApplication()->Handle());

  prevKeys = new ubyte[256];
  ZKeyboard::GetKeys(prevKeys);

  //D3DX10CreateFont(graphics->GetDevice(), 25, 0, FW_DONTCARE, 1, 
  //  FALSE, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, 1,
  //  DEFAULT_PITCH | FF_DONTCARE,
  //  TEXT("Arial"),
  //  &itemFont);

  //MenuItem mItem(MenuOptions::MenuOptionPlay, itemFont, "Play", 100, 100);
  //mItem.Initialize();
  //menuItems.push_back(mItem);
  
}

void MenuState::Activate()
{
  SetNextState(Menu);
  if(prevKeys)
    ZKeyboard::GetKeys(prevKeys);
}

void MenuState::Deactivate()
{

}



void MenuState::Update(double delta)
{
  newKeys = new ubyte[256];
  ZKeyboard::GetKeys(newKeys);
  ZMouse::GetMouseState(newMState);

  if(newKeys[Keys::Esc] && !prevKeys[Keys::Esc])
  {
    SetNextState(ApplicationStates::Quit);
  }

  if(ZKeyboard::IsKeyDown(Keys::P))
  {
    SetNextState(ApplicationStates::Game);
  }
  
  for(MenuItem& menuItem : menuItems)
  {
    menuItem.Update(delta);

    if(ZCollisions::RectPoint(menuItem.GetTextRect(), newMState.x, newMState.y))
    {
      menuItem.SetSelected(true);
      if(newMState.leftButton && !prevMState.leftButton)
      {
        // Menu item was clicked
        DoMenuItemAction(menuItem);
      }
    }
    else
    {
      menuItem.SetSelected(false);
    }
  }

  if(prevKeys) delete[] prevKeys;
    prevKeys = newKeys;

  prevMState = newMState;
}

void MenuState::Draw()
{
  //graphics->Clear(D3DXCOLOR(0, 0, 0, 1), true);

  for(MenuItem& menuItem : menuItems)
  {
    //menuItem.Draw();
  }

  //GetApplication()->DisplayFps();

 // graphics->Present(true);
}


void MenuState::DoMenuItemAction(const MenuItem& menuItem)
{
  switch(menuItem.GetType())
  {
  case MenuOptions::MenuOptionPlay:
    SetNextState(ApplicationStates::Game);
    return;
  case MenuOptions::MenuOptionOptions:
    SetNextState(ApplicationStates::Menu);
    return;
  case MenuOptions::MenuOptionQuit:
    SetNextState(ApplicationStates::Quit);
    return;
  default:
    MessageBox(NULL, "Invalid menu option detected", "Error", MB_OK | MB_ICONERROR);
  }

}