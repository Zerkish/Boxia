#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

class ZGraphics;
class WorldManager;
class GameState;

class GameManager
{
public:
  // Constructor
  GameManager(GameState* gameState);
  void Initialize();

  void PauseGame();
  void StartGame();
  void ResumeGame();
  
  void Update(double delta);

  void Draw();

private:
  GameState* gameState;
  ZGraphics* graphics;
};

#endif