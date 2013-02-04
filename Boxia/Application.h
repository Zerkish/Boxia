#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H
#include <ZD3DApp.h>
#include <d3d10.h>
#include <D3DX10.h>
#include <vector>
class ZCamera;
class ZGraphics;

using std::vector;
class Chunk;

class Application : public ZD3DApp
{
public:
  Application(HINSTANCE hInst, int width, int height);

  bool Initialize();

  bool Run() { return ZD3DApp::Run(); }
  
  void Update(double delta);
  void Draw();

  void CreateTestBoxes();

  ~Application();
private:
  ZCamera* camera;
  ID3D10Effect* effect;
  ID3D10InputLayout* lVPositionColor;
  ID3D10EffectTechnique* basicTechnique;
  D3D10_TECHNIQUE_DESC basicTechDesc;
  D3DXMATRIX projection;
  ID3D10RasterizerState* rs;
  ID3D10EffectMatrixVariable* fxWVP;
  //Chunk* chunk;
  vector<Chunk*> chunks;
  ZGraphics* graphics;
  D3DXVECTOR4 lightPos;
};

#endif