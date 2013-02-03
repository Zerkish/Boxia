#include "Application.h"


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, char* cmd, int cmdShow)
{
  Application app(hInst, 1280, 780);
  app.Initialize();
  app.Run();

}