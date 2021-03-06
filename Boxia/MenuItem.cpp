#include "MenuItem.h"
#include <LibZerkish.h>

D3DXCOLOR MenuItem::selectedColor = D3DXCOLOR(0.75f, 0.75f, 0.05f, 1.0f);
D3DXCOLOR MenuItem::normalColor = D3DXCOLOR(1.0f, 0.05f, 0.05f, 1.0f);

MenuItem::MenuItem(MenuOptions type, ID3DX10Font* font, const char* menuStr, int x, int y)
{
  this->font = font;
  this->type = type;
  text = string(menuStr);
  font->AddRef();
  textPosX = x;
  textPosY = y;
  
}

void MenuItem::Initialize()
{
  TEXTMETRICA tmet;
  font->GetTextMetricsA(&tmet);
  int tWidth = static_cast<int>(tmet.tmAveCharWidth * text.size() * 1.5f);
  int tHeight = tmet.tmHeight;
  textRect.top = textPosY;
  textRect.left = textPosX;
  textRect.bottom = textPosY + tHeight;
  textRect.right = textPosX + tWidth;
  selected = false;  
}

void MenuItem::Update(double delta)
{

}

void MenuItem::Draw()
{
  color = selected ? selectedColor : normalColor; 
  font->DrawTextA(NULL, text.c_str(), text.size(), &textRect, DT_LEFT, color);
}




