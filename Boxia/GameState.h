#ifndef GAMESTATE_H
#define GAMESTATE_H
#include <LibZerkish.h>
#include "ApplicationState.h"

#include <d3d10.h>
#include <dxgi.h>
#include <d3dx10.h>


class GameManager;
class WorldManager;
class ZCamera;

struct ID3D10Effect;

class GameState : public ApplicationState
{
public:
  GameState(ZD3DApp* application);
  ~GameState();
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
  void CreateWorldInputLayout();
  void LoadDefaultEffect();

  // Test
  void MoveTestCam(double delta);
  ZCamera* camera;
  float camXZRot;
  float camYRot;
  float camDist;

  bool gameRunning;
  bool gamePaused;
  GameManager* gameManager;
  WorldManager* worldManager;
  ZGraphics* graphics;
  ID3D10Effect* effect;
  D3DXMATRIX projection;

  // World Vars
  ID3D10InputLayout* worldInputLayout;
};

// INLINES
inline ZGraphics* GameState::GetGraphics() const
{
  return graphics;
}
#endif