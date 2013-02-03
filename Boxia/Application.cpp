#include "Application.h"
#include <LibZerkish.h>
#include <D3DX10.h>
#include "Chunk.h"

float camXZRot;
float camYRot;


#define PI 3.14159
float camDist = 40;
int chunkCount = 0;

Application::Application(HINSTANCE hInst, int width, int height)
  :ZD3DApp(hInst, width, height)
{
  camXZRot = 0;
  camYRot = 0;
}


bool Application::Initialize()
{
  if(!ZD3DApp::Initialize())
    return false;

  
  camera = new ZCamera(D3DXVECTOR3(0, 0, -50), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 1, 0));
  camera->UpdateViewMatrix();

  ID3D10Blob* compErrors = NULL;

  HRESULT hr = D3DX10CreateEffectFromFile(
    "simpleShader.fx",
    NULL,
    NULL,
    "fx_4_0",
    D3D10_SHADER_ENABLE_STRICTNESS,
    0,
    graphicsDevice,
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
  }
  graphics = new ZGraphics(graphicsDevice, effect);
  basicTechnique = effect->GetTechniqueByName("Renderer");

  basicTechnique->GetDesc(&basicTechDesc);

  D3DXMatrixPerspectiveFovLH(&projection, 3.14159f / 3.0f, 1280.0f / 780.0f, 0.01f, 1000.0f);
  effect->GetVariableByName("projection")->AsMatrix()->SetMatrix(projection);

  MeasureFps(true);

  D3D10_RASTERIZER_DESC normalDesc;
  ZeroMemory(&normalDesc, sizeof(D3D10_RASTERIZER_DESC));
  normalDesc.AntialiasedLineEnable = true;
  normalDesc.CullMode = D3D10_CULL_BACK;
  normalDesc.DepthClipEnable = true;
  normalDesc.DepthBias = 1;
  normalDesc.FillMode = D3D10_FILL_SOLID;
  graphicsDevice->CreateRasterizerState(&normalDesc, &rs);

  for(int i = 0; i < 64; ++i)
  {
    chunks.push_back(new Chunk(graphics));
    chunks[i]->Init();
    chunks[i]->SetWorldPosition(
      ((i % 8) - 4) * 16,
      0,
      ((i / 8) - 4) * 16);
  }

  return true;
}


void Application::Update(double delta)
{
  if(ZKeyboard::IsKeyDown(Keys::Left))
  {
    camXZRot -= 0.001f * delta;
  }
  else if(ZKeyboard::IsKeyDown(Keys::Right))
  {
    camXZRot += 0.001f * delta;
  }

  if(ZKeyboard::IsKeyDown(Keys::Up))
  {
    camYRot += 0.001f * delta;
  }
  else if(ZKeyboard::IsKeyDown(Keys::Down))
  {
    camYRot -= 0.001f * delta;
  }

  

  if(camYRot > PI)
  {
    camYRot = -PI;
  }

  if(camXZRot > PI)
  {
    camXZRot = -PI;
  }

  //if(ZKeyboard::IsKeyDown(Keys::Space))
  //{
  //  chunks.push_back(new Chunk(graphics));
  //  chunks[chunkCount]->Init();
  //  chunks[chunkCount]->SetWorldPosition(
  //    ((chunkCount % 8) - 4) * 16,
  //    0,
  //    ((chunkCount / 8) - 4) * 16);
  //  chunkCount++;
  //}

  if(ZKeyboard::IsKeyDown(Keys::W))
  {
    camDist -= 0.1f * delta;
  }
  if(ZKeyboard::IsKeyDown(Keys::S))
  {
    camDist += 0.1f * delta;
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


void Application::Draw()
{
  graphicsDevice->ClearRenderTargetView(renderTargetView, D3DXCOLOR(0.30f, 0.35f, 0.80f, 1));
  graphicsDevice->ClearDepthStencilView(depthStencilView, D3D10_CLEAR_DEPTH|D3D10_CLEAR_STENCIL, 1.0f, 0);
  
  float blendFac[] = { 1.0f, 1.0f, 1.0f, 1.0f};
  graphicsDevice->OMSetBlendState(0, blendFac, 0xffffffff);

  graphicsDevice->RSSetState(rs);
  effect->GetVariableByName("view")->AsMatrix()->SetMatrix((float*)&camera->ViewMatrix());
  
  // Draw Boxes
  graphicsDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  graphicsDevice->OMSetDepthStencilState(0, 0);

  basicTechnique->GetDesc(&basicTechDesc);

  //ID3D10EffectVectorVariable* lightDir = effect->GetVariableByName("diffuseDir")->AsVector();

  //D3DXVECTOR3 invDiff = -camera->Position();
  //D3DXVec3Normalize(&invDiff, &invDiff);
  //lightDir->SetFloatVector((float*)&invDiff);

  for(Chunk* chunk : chunks)
  {
    chunk->Draw();
  }

  DisplayFps();

  swapChain->Present(0, 0);
}

void Application::CreateTestBoxes()
{

}


Application::~Application()
{
  if(effect)
    effect->Release();

  delete camera;

}