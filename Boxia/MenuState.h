#ifndef MENUSTATE_H
#define MENUSTATE_H

#include "ApplicationState.h"
#include <vector>
#include "MenuItem.h"

class ID3DX10Font;
class ZGraphics;

typedef unsigned char ubyte;

class MenuState : public ApplicationState
{
public:
  MenuState(ZD3DApp* application);

  void Activate();
  void Deactivate();
  void Initialize();
  void Update(double delta);
  void Draw();
protected:

private:
  ID3DX10Font* itemFont;
  std::vector<MenuItem> menuItems;
  ZGraphics* graphics;
  ubyte* prevKeys;
  ubyte* newKeys;
};

#endif