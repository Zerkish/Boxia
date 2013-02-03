#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

struct ID3D10Device;

class GameManager
{
public:
  // Constructor
  GameManager();

  void Update();

  void Draw();

private:
  ID3D10Device* device;
};

#endif