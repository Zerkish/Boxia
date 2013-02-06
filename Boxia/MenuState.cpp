#include "MenuState.h"
#include <ZGraphics.h>
#include <ZD3DApp.h>
#include <ZKeyboard.h>
#include <D3DX10.h>

MenuState::MenuState(ZD3DApp* application)
  :ApplicationState(application)
{
  SetNextState(Menu);
}

void MenuState::Initialize()
{
  graphics = new ZGraphics(GetApplication()->GetGraphicsDevice());
  prevKeys = new ubyte[256];
  ZKeyboard::GetKeys(prevKeys);

  D3DX10CreateFont(graphics->GetDevice(), 25, 0, FW_DONTCARE, 1, 
    FALSE, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, 1,
    DEFAULT_PITCH | FF_DONTCARE,
    TEXT("Arial"),
    &itemFont);

  MenuItem mItem(itemFont, "Play", 100, 100);
  mItem.Initialize();
  menuItems.push_back(mItem);
  
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

  if(newKeys[Keys::Esc] && !prevKeys[Keys::Esc])
  {
    SetNextState(Quit);
  }
  if(ZKeyboard::IsKeyDown(Keys::P))
  {
    SetNextState(Game);
  }
  
  if(prevKeys) delete[] prevKeys;
  prevKeys = newKeys;

  for(MenuItem& menuItem : menuItems)
  {
    menuItem.Update(delta);
  }
  
}

void MenuState::Draw()
{
  graphics->GetDevice()->ClearRenderTargetView(GetApplication()->GetRenderTarget(), D3DXCOLOR(0, 0, 0, 1));
  for(MenuItem& menuItem : menuItems)
  {
    menuItem.Draw();
  }

  GetApplication()->DisplayFps();

  GetApplication()->GetSwapChain()->Present(0, 0);
}