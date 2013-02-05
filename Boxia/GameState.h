#ifndef GAMESTATE_H
#define GAMESTATE_H
#include "ApplicationState.h"
#include <D3DX10.h>

class GameManager;
class ZGraphics;
class ZD3DApp;
class WorldManager;
class ZCamera;

struct ID3D10Effect;

class GameState : public ApplicationState
{
public:
  GameState(ZD3DApp* application);

  void Initialize();
  
  void Activate();
  void Deactivate();
  void StartGame();
  void PauseGame();
  void ResumeGame();

  void Update(double delta);
  void Draw();

  #pragma region Accessors
  ZGraphics* GetGraphics() const;
  #pragma endregion

protected:

private:
  void LoadDefaultEffect();

  bool gameRunning;
  bool gamePaused;
  GameManager* gameManager;
  WorldManager* worldManager;
  ZGraphics* graphics;
  ID3D10Effect* effect;
  ZCamera* camera;
  D3DXMATRIX projection;
};

// INLINES
inline ZGraphics* GameState::GetGraphics() const
{
  return graphics;
}
#endif