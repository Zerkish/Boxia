#include "Cube.h"

#include "VertexFormats.h"

Cube::Cube(ID3D10Device* device, float scale)
{
  this->graphicsDevice = device;
}

void Cube::Init()
{
  VPositionColor vertices[] = 
  {
    {D3DXVECTOR3(-1.0f, -1.0f, -1.0f), D3DXCOLOR(1,1,1,1)},
    {D3DXVECTOR3(-1.0f, +1.0f, -1.0f), D3DXCOLOR(1,0,0,0)},
    {D3DXVECTOR3(+1.0f, +1.0f, -1.0f), D3DXCOLOR(1,1,0,0)},
    {D3DXVECTOR3(+1.0f, -1.0f, -1.0f), D3DXCOLOR(1,0,1,0)},
    {D3DXVECTOR3(-1.0f, -1.0f, +1.0f), D3DXCOLOR(1,0,0,1)},
    {D3DXVECTOR3(-1.0f, +1.0f, +1.0f), D3DXCOLOR(1,1,1,0)},
    {D3DXVECTOR3(+1.0f, +1.0f, +1.0f), D3DXCOLOR(1,0,1,1)},
    {D3DXVECTOR3(+1.0f, -1.0f, +1.0f), D3DXCOLOR(1,1,0,1)}
  };

  this->vertices = new VPositionColor[8];
  for(int i = 0; i < 8; ++i)
  {
    this->vertices[i] = vertices[i];
  }

  D3D10_BUFFER_DESC vbd;
  vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
  vbd.ByteWidth = sizeof(VPositionColor) * 8;
  vbd.CPUAccessFlags = 0;
  vbd.MiscFlags = 0;
  vbd.Usage = D3D10_USAGE_IMMUTABLE;

  D3D10_SUBRESOURCE_DATA sbd;
  sbd.pSysMem = this->vertices;

  graphicsDevice->CreateBuffer(&vbd, &sbd, &vBuffer);

  int indices[] = 
  {
    // Front
    0, 1, 2,
    0, 2, 3,

    // Back
    4, 6, 5,
    4, 7, 6,

    // left
    4, 5, 1,
    4, 1, 0,

    // right
    3, 2, 6,
    3, 6, 7,

    // top,
    1, 5, 6,
    1, 6, 2,

    // bottom
    4, 0, 3,
    4, 3, 7
  };

  this->indices = new int[36];
  for(int i = 0; i < 36; ++i)
  {
    this->indices[i] = indices[i];
  }

  D3D10_BUFFER_DESC ibd;
  ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
  ibd.ByteWidth = sizeof(int) * 36;
  ibd.CPUAccessFlags = 0;
  ibd.MiscFlags = 0;
  ibd.Usage = D3D10_USAGE_IMMUTABLE;

  D3D10_SUBRESOURCE_DATA isub;
  isub.pSysMem = this->indices;

  graphicsDevice->CreateBuffer(&ibd, &isub, &iBuffer);
}

void Cube::Draw()
{
  UINT stride = sizeof(VPositionColor);
  UINT offset = 0;
  //graphicsDevice->OMSetDepthStencilState(0, 0);
  float blendFac[] = {1.0f, 0.0f, 0.0f, 1.0f};
  graphicsDevice->OMSetBlendState(0, blendFac, 0xffffffff);
  
  graphicsDevice->IASetVertexBuffers(0, 1, &vBuffer, &stride, &offset);
  graphicsDevice->IASetIndexBuffer(iBuffer, DXGI_FORMAT_R32_UINT, 0);
  graphicsDevice->DrawIndexed(12 * 3, 0, 0);
}

void Cube::SetPosition(float x, float y, float z)
{
  D3DXMatrixTranslation(&matWorld, x, y, z);
}

Cube::~Cube()
{
  vBuffer->Release();
  iBuffer->Release();
}