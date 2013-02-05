#ifndef MENUSTATE_H
#define MENUSTATE_H

#include "ApplicationState.h"

class ZGraphics;

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
  ZGraphics* graphics;
};

#endif