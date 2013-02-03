#ifndef CUBE_H
#define CUBE_H

#include <d3d10.h>
#include <D3DX10.h>

struct VPositionColor;

class Cube
{
public:
  Cube(ID3D10Device* device, float scale);
  void Init();
  void Draw();

  void SetPosition(float x, float y, float z);

  ~Cube();
  
  VPositionColor* vertices;
  int* indices;
  D3DXMATRIX matWorld;
  ID3D10Buffer* vBuffer;
  ID3D10Buffer* iBuffer;

protected:

private:
  ID3D10Device* graphicsDevice;
};


#endif