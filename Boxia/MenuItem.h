#ifndef MYMENU_H
#define MYMENU_H


#include <d3d10.h>
#include <d3dx10.h>
#include <string>
struct ID3DX10Font;

using std::string;

enum MenuOptions
{
  MenuOptionPlay,
  MenuOptionOptions,
  MenuOptionQuit,
};

class MenuItem
{
public:
  MenuItem(MenuOptions type, ID3DX10Font* font, const char* menuStr, int x, int y);
  
  void Initialize();

  const RECT& GetTextRect() const;
  bool GetSelected() const;
  void SetSelected(bool value);
  MenuOptions GetType() const;
  void Update(double delta);

  void Draw();

private:
  static D3DXCOLOR selectedColor;
  static D3DXCOLOR normalColor;
  
  MenuOptions type;
  bool selected;
  int textPosX;
  int textPosY;
  RECT textRect;
  ID3DX10Font* font;
  D3DXCOLOR color;
  string text;
};


// INLINES
inline const RECT& MenuItem::GetTextRect() const
{
  return textRect;
}

inline bool MenuItem::GetSelected() const
{
  return selected;
}

inline void MenuItem::SetSelected(bool value)
{
  selected = value;
}

inline MenuOptions MenuItem::GetType() const
{
  return type;
}


#endif