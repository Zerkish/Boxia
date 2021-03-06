#include "Application.h"
#include <LibZerkish.h>
#include <D3DX10.h>
#include "Chunk.h"
#include "StateManager.h"

float camXZRot;
float camYRot;


#define PI 3.14159f
float camDist = 40;
int chunkCount = 0;

Application::Application(HINSTANCE hInst, int width, int height)
  :ZD3DApp(hInst, width, height)
{
  camXZRot = 0;
  camYRot = 0;
  lightPos = D3DXVECTOR4(0, 5, 0, 0);
}


bool Application::Initialize()
{
  if(!ZD3DApp::Initialize())
    return false;

  
  //camera = new ZCamera(D3DXVECTOR3(0, 0, -50), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 1, 0));
  //camera->UpdateViewMatrix();

  //ID3D10Blob* compErrors = NULL;

  //HRESULT hr = D3DX10CreateEffectFromFile(
  //  "simpleShader.fx",
  //  NULL,
  //  NULL,
  //  "fx_4_0",
  //  D3D10_SHADER_ENABLE_STRICTNESS,
  //  0,
  //  graphicsDevice,
  //  NULL,
  //  NULL,
  //  &effect,
  //  &compErrors,
  //  NULL
  //);

  //if(hr != S_OK)
  //{
  //  MessageBox(0, "Shader error", "SH err", MB_OK);
  //}

  //if(compErrors)
  //{
  //  MessageBox(NULL, (char*)compErrors->GetBufferPointer(),
  //    "Shader Compilation Error", MB_OK | MB_ICONERROR);
  //  compErrors->Release();
  //  exit(0);
  //}
  ////graphics = new ZGraphics(graphicsDevice, effect);
  //graphics = new ZGraphics();
  //graphics->InitializeDefault(Handle());
  //basicTechnique = effect->GetTechniqueByName("Renderer");

  //basicTechnique->GetDesc(&basicTechDesc);

  //D3DXMatrixPerspectiveFovLH(&projection, 3.14159f / 3.0f, 1280.0f / 780.0f, 0.01f, 1000.0f);
  //effect->GetVariableByName("projection")->AsMatrix()->SetMatrix(projection);

  //MeasureFps(true);

  //D3D10_RASTERIZER_DESC normalDesc;
  //ZeroMemory(&normalDesc, sizeof(D3D10_RASTERIZER_DESC));
  //normalDesc.AntialiasedLineEnable = true;
  //normalDesc.CullMode = D3D10_CULL_BACK;
  //normalDesc.DepthClipEnable = true;
  //normalDesc.DepthBias = 0;
  //normalDesc.FillMode = D3D10_FILL_SOLID;
  //normalDesc.MultisampleEnable = true;

  //graphicsDevice->CreateRasterizerState(&normalDesc, &rs);

  stateManager = new StateManager(this);
  stateManager->Initialize();

  //graphics = new ZGraphics();
  //graphics->device = graphicsDevice;
  ////graphics->InitializeDefault(Handle());
  //graphics->SetEffect(effect);

  //for(int i = 0; i < 64; ++i)
  //{
  //  chunks.push_back(new Chunk());
  //  chunks[i]->Init();
  //  chunks[i]->SetWorldPosition(
  //    static_cast<float>(((i % 8) - 4) * 16),
  //    0.0f,
  //    static_cast<float>(((i / 8) - 4) * 16)
  //  );
  //}

  return true;
}


void Application::Update(double delta)
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
  
  if(ZKeyboard::IsKeyDown(Keys::Numpad8))
  {
    lightPos.z += 0.01f * mtime;
  }
  if(ZKeyboard::IsKeyDown(Keys::Numpad5))
  {
    lightPos.z -= 0.01f * mtime;
  }

  if(ZKeyboard::IsKeyDown(Keys::Numpad4))
  {
    lightPos.x -= 0.01f * mtime;
  }
  if(ZKeyboard::IsKeyDown(Keys::Numpad6))
  {
    lightPos.x += 0.01f * mtime;
  }

  if(ZKeyboard::IsKeyDown(Keys::Subtract))
  {
    lightPos.y += 0.01f * mtime;
  }

  if(ZKeyboard::IsKeyDown(Keys::Add))
  {
    lightPos.y -= 0.01f * mtime;
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
  
//  camera->SetPosition(camPos);
  stateManager->Update(delta);
}


void Application::Draw()
{


  //graphicsDevice->ClearRenderTargetView(renderTargetView, D3DXCOLOR(0.30f, 0.35f, 0.80f, 1));
  //graphicsDevice->ClearDepthStencilView(depthStencilView, D3D10_CLEAR_DEPTH|D3D10_CLEAR_STENCIL, 1.0f, 0);
  //
  //float blendFac[] = { 1.0f, 1.0f, 1.0f, 1.0f};
  //graphicsDevice->OMSetBlendState(0, blendFac, 0xffffffff);

  //graphicsDevice->RSSetState(rs);
  //effect->GetVariableByName("view")->AsMatrix()->SetMatrix((float*)&camera->ViewMatrix());
  //
  //// Draw Boxes
  //graphicsDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  //graphicsDevice->OMSetDepthStencilState(0, 0);

  //basicTechnique->GetDesc(&basicTechDesc);

  //ID3D10EffectVectorVariable* camPos = effect->GetVariableByName("cameraPosition")->AsVector();


  //camPos->SetFloatVector((float*)&camera->Position());

  //camPos = effect->GetVariableByName("pointLightPos")->AsVector();
  //camPos->SetFloatVector((float*)&lightPos);

  stateManager->Draw();
  
  //DisplayFps();

  //swapChain->Present(0, 0);

  return;

  //for(Chunk* chunk : chunks)
  //{
  //  chunk->Draw(graphics);
  //}

  //DisplayFps();

  //swapChain->Present(0, 0);
}


Application::~Application()
{
}