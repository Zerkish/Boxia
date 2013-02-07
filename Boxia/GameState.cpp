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
  graphics = new ZGraphics();
  graphics->InitializeDefault(GetApplication()->Handle());
  LoadDefaultEffect();
  graphics->SetEffect(effect);

  gameManager = new GameManager(this);
  gameManager->Initialize();

  // Creates the input layout used to render chunks and blocks.
  CreateWorldInputLayout();

  worldManager = new WorldManager(graphics);
  worldManager->Initialize(worldInputLayout);
  worldManager->FillWorld();

  camera = new ZCamera(D3DXVECTOR3(0, 0, -50), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 1, 0));
  camera->UpdateViewMatrix();
  camDist = -50;
  camXZRot = 0;
  camYRot = 0;

  D3DXMatrixPerspectiveFovLH(&projection, PI * 0.30f, 1280 / 780.0f, 0.01f, 1000.0f); 
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

void GameState::CreateWorldInputLayout()
{
  D3D10_INPUT_ELEMENT_DESC desc[4];
  desc[0].AlignedByteOffset = 0;
  desc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
  desc[0].InputSlot = 0;
  desc[0].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
  desc[0].InstanceDataStepRate = 0;
  desc[0].SemanticIndex = 0;
  desc[0].SemanticName = "POSITION";

  desc[1].AlignedByteOffset = 12;
  desc[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
  desc[1].InputSlot = 0;
  desc[1].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
  desc[1].InstanceDataStepRate = 0;
  desc[1].SemanticIndex = 0;
  desc[1].SemanticName = "NORMAL";

  desc[2].AlignedByteOffset = 24;
  desc[2].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
  desc[2].InputSlot = 0;
  desc[2].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
  desc[2].InstanceDataStepRate = 0;
  desc[2].SemanticIndex = 0;
  desc[2].SemanticName = "COLOR";

  desc[3].AlignedByteOffset = 40;
  desc[3].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
  desc[3].InputSlot = 0;
  desc[3].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
  desc[3].InstanceDataStepRate = 0;
  desc[3].SemanticIndex = 1;
  desc[3].SemanticName = "COLOR";


  worldInputLayout = graphics->CreateInputLayout(effect, desc, 4);
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
  if(ZKeyboard::IsKeyDown(Keys::Esc))
  {
    SetNextState(Menu);
  }

  if(gamePaused || !gameRunning)
    return;

  gameManager->Update(delta);
  worldManager->UpdateWorld(delta);

  MoveTestCam(delta);
}

void GameState::Draw()
{
  // Clear rendertarget and depthstencil
  graphics->Clear(D3DXCOLOR(0, 0, 0, 1), true);
  graphics->GetDevice()->OMSetDepthStencilState(0, 0);
  float blendFac[] = { 1.0f, 1.0f, 1.0f, 1.0f};
  graphics->GetDevice()->OMSetBlendState(0, blendFac, 0xffffffff);

  graphics->SetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  effect->GetVariableByName("view")->AsMatrix()->SetMatrix((float*)&camera->ViewMatrix());

  worldManager->DrawWorld();
  gameManager->Draw();

  //GetApplication()->DisplayFps();
  graphics->Present(false);
}

void GameState::MoveTestCam(double delta)
{
  float mtime = static_cast<float>(delta);
  if(ZKeyboard::IsKeyDown(Keys::Left))
  {
    camXZRot -= 0.001f * mtime;
  }
  else if(ZKeyboard::IsKeyDown(Keys::Right))
  {
    camXZRot += 0.001f * mtime;
  }

  if(ZKeyboard::IsKeyDown(Keys::Up))
  {
    camYRot += 0.001f * mtime;
  }
  else if(ZKeyboard::IsKeyDown(Keys::Down))
  {
    camYRot -= 0.001f * mtime;
  }

  

  if(camYRot > PI)
  {
    camYRot = -PI;
  }

  if(camXZRot > PI)
  {
    camXZRot = -PI;
  }

  if(ZKeyboard::IsKeyDown(Keys::W))
  {
    camDist -= 0.1f * mtime;
  }
  if(ZKeyboard::IsKeyDown(Keys::S))
  {
    camDist += 0.1f * mtime;
  }

  D3DXVECTOR3 camTarget(0, 0, 1);

  D3DXMATRIX xzRot;
  D3DXMATRIX yRot;

  D3DXMatrixRotationY(&xzRot, camXZRot);
  
  D3DXVECTOR3 xAxis(1, 0, 0);
  D3DXVECTOR4 res;
  D3DXVec3Transform(&res, &xAxis, &xzRot);
  xAxis = D3DXVECTOR3(res.x, res.y, res.z);
  D3DXMatrixRotationAxis(&yRot, &xAxis, camYRot);

  D3DXMATRIX comb = xzRot * yRot;

  D3DXVECTOR3 camPos(0, 0, -camDist);
  D3DXVECTOR4 camRes;
  D3DXVec3Transform(&camRes, &camPos, &comb);

  camPos = D3DXVECTOR3(camRes.x, camRes.y, camRes.z);
  
  camera->SetPosition(camPos);
}