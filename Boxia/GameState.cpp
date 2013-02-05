#include "GameState.h"
#include "GameManager.h"
#include <LibZerkish.h>
#include "WorldManager.h"

GameState::GameState(ZD3DApp* application)
  :ApplicationState(application)
{

}

void GameState::Activate()
{
  SetNextState(Game);
}

void GameState::Deactivate()
{

}

void GameState::Initialize()
{
  graphics = new ZGraphics(GetApplication()->GetGraphicsDevice());
  LoadDefaultEffect();
  graphics->SetEffect(effect);

  gameManager = new GameManager(this);
  gameManager->Initialize();

  worldManager = new WorldManager();
  worldManager->FillWorld(graphics);

  camera = new ZCamera(D3DXVECTOR3(0, 0, -50), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 1, 0));
  camera->UpdateViewMatrix();

  D3DXMatrixPerspectiveFovLH(&projection, 3.14159f * 0.25f, 1280 / 780.0f, 0.1f, 1000.0f); 
  effect->GetVariableByName("projection")->AsMatrix()->SetMatrix(projection);
}

void GameState::LoadDefaultEffect()
{
  ID3D10Blob* compErrors = NULL;

  HRESULT hr = D3DX10CreateEffectFromFile(
    "simpleShader.fx",
    NULL,
    NULL,
    "fx_4_0",
    D3D10_SHADER_ENABLE_STRICTNESS,
    0,
    graphics->GetDevice(),
    NULL,
    NULL,
    &effect,
    &compErrors,
    NULL
  );

  if(hr != S_OK)
  {
    MessageBox(0, "Shader error", "SH err", MB_OK);
  }

  if(compErrors)
  {
    MessageBox(NULL, (char*)compErrors->GetBufferPointer(),
      "Shader Compilation Error", MB_OK | MB_ICONERROR);
    compErrors->Release();
    exit(0);
  }
}

void GameState::StartGame()
{
  gamePaused = false;
  gameRunning = true;

  gameManager->StartGame();
}

void GameState::PauseGame()
{
  gamePaused = true;
  gameManager->PauseGame();
}

void GameState::ResumeGame()
{
  gamePaused = false;
  gameManager->ResumeGame();
}

void GameState::Update(double delta)
{
  if(gamePaused || !gameRunning)
    return;

  gameManager->Update(delta);
}

void GameState::Draw()
{
  effect->GetVariableByName("view")->AsMatrix()->SetMatrix((float*)&camera->ViewMatrix());

  worldManager->DrawWorld();
  gameManager->Draw();
}