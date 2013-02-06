#ifndef MYMENU_H
#define MYMENU_H

#include <D3DX10.h>
#include <string>

struct ID3DX10Font;

using std::string;


class MenuItem
{
public:
  MenuItem(ID3DX10Font* font, const char* menuStr, int x, int y);

  void Initialize();

  bool GetSelected() const;
  void SetSelected(bool value);
  void Update(double delta);

  void Draw();

private:
  static D3DXCOLOR selectedColor;
  static D3DXCOLOR normalColor;
  bool selected;
  int textPosX;
  int textPosY;
  RECT textRect;
  ID3DX10Font* font;
  D3DXCOLOR color;
  string text;
};


// INLINES
inline bool MenuItem::GetSelected() const
{
  return selected;
}

inline void MenuItem::SetSelected(bool value)
{
  selected = value;
}


#endif