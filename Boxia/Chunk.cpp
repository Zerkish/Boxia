#include "Chunk.h"
#include <d3d10.h>
#include "Block.h"
#include <cassert>

ID3D10InputLayout* Chunk::chunkILayout = NULL;

void Chunk::CreateChunkILayout(ZGraphics* g)
{
  D3D10_INPUT_ELEMENT_DESC desc[3];
  desc[0].AlignedByteOffset = 0;
  desc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
  desc[0].InputSlot = 0;
  desc[0].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
  desc[0].InstanceDataStepRate = 0;
  desc[0].SemanticIndex = 0;
  desc[0].SemanticName = "POSITION";

  desc[1].AlignedByteOffset = 12;
  desc[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
  desc[1].InputSlot = 0;
  desc[1].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
  desc[1].InstanceDataStepRate = 0;
  desc[1].SemanticIndex = 0;
  desc[1].SemanticName = "COLOR";

  desc[2].AlignedByteOffset = 28;
  desc[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
  desc[2].InputSlot = 0;
  desc[2].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
  desc[2].InstanceDataStepRate = 0;
  desc[2].SemanticIndex = 0;
  desc[2].SemanticName = "NORMAL";

  Chunk::chunkILayout = g->CreateInputLayot(desc, 3);
}

Chunk::Chunk(ZGraphics* graphics)
{
  this->graphics = graphics;
  blocks = nullptr;
  mesh = nullptr;
  //vBuffer = nullptr;
  //iBuffer = nullptr;
}


bool Chunk::Init()
{
  if(Chunk::chunkILayout == NULL)
    CreateChunkILayout(graphics);

  worldPos = D3DXVECTOR3(0, 0, 0);
  mesh = new ZMesh(graphics, chunkILayout);
  mesh->SetPosition(0, 0, 0);

  float map[gkChunkSize][gkChunkSize];
  for(int x = 0; x < gkChunkSize; ++x)
  {
    for(int z = 0; z < gkChunkSize; ++z)
    {
      map[x][z] = (rand() % 8) * (float)((rand() % 100) / 100.0f);
    }
  }

  for(int x = 0; x < gkChunkSize - 1; ++x)
  {
    for(int z = 0; z < gkChunkSize - 1; ++z)
    {
      float h = map[x][z] + (map[x + 1][z + 1] - map[x][z]) * 0.5f;
      map[x][z] = h;
    }
  }

  blocks = new Block**[gkChunkSize];
  for(int x = 0; x < gkChunkSize; ++x)
  {
    blocks[x] = new Block*[gkChunkSize];
    for(int z = 0; z < gkChunkSize; ++z)
    {
      blocks[x][z] = new Block[gkChunkSize];
      for(int y = 0; y < gkChunkSize; ++y)
      {
        blocks[x][z][y].SetVisible(true);

        if(y > map[x][z])
        {
          blocks[x][z][y].SetVisible(false);
        }
        if(y >= 0 && y < 1)
        {
          blocks[x][z][y].BlockType = BlockTypes::Rock;
        }
        else if(y < 2)
        {
          blocks[x][z][y].BlockType = BlockTypes::Dirt;
        }
        else
        {
          blocks[x][z][y].BlockType = BlockTypes::Grass;
        }
      }
    }
  }

  CreateAndFillBuffers();

  return true;
}

void Chunk::CreateAndFillBuffers()
{

  mesh->BeginMesh();
  for(int x = 0; x < gkChunkSize; ++x)
  {
    for(int z = 0; z < gkChunkSize; ++z)
    {
      for(int y = 0; y < gkChunkSize; ++y)
      {
        D3DXVECTOR4 blockCol = GetBlockColor(blocks[x][z][y]);
        
        if(blocks[x][z][y].Visible() == false)
        {
          continue;
        }

        int aX = x - gkChunkSize / 2, aY = y - gkChunkSize / 2, aZ = z - gkChunkSize / 2;

        D3DXVECTOR3 blockPos(worldPos.x + aX, worldPos.y + aY, worldPos.z + aZ);
       
        int hideFlags = 0;
        if(x > 0 && blocks[x - 1][z][y].Visible())
          hideFlags |= HideFlags::Left;
        if(x < gkChunkSize - 1 && blocks[x + 1][z][y].Visible())
          hideFlags |= HideFlags::Right;
        if(z > 0 && blocks[x][z - 1][y].Visible())
          hideFlags |= HideFlags::Front;
        if(z < gkChunkSize - 1 && blocks[x][z + 1][y].Visible())
          hideFlags |= HideFlags::Back;
        if(y > 0 && blocks[x][z][y - 1].Visible())
          hideFlags |= HideFlags::Bottom;
        if(y < gkChunkSize - 1 && blocks[x][z][y + 1].Visible())
          hideFlags |= HideFlags::Top;

        CreateBlock(worldPos.x + aX, worldPos.y + aY, worldPos.z + aZ, blockCol, hideFlags);
      }
    }
  }
  mesh->FinishMesh();

}

void Chunk::CreateBlock(float x, float y, float z, D3DXVECTOR4& color, int hideFlags)
{
  D3DXVECTOR3 p0(x - gkBlockSize / 2, y + gkBlockSize / 2, z - gkBlockSize / 2);
  D3DXVECTOR3 p1(x + gkBlockSize / 2, y + gkBlockSize / 2, z - gkBlockSize / 2);
  D3DXVECTOR3 p2(x + gkBlockSize / 2, y - gkBlockSize / 2, z - gkBlockSize / 2);
  D3DXVECTOR3 p3(x - gkBlockSize / 2, y - gkBlockSize / 2, z - gkBlockSize / 2);
  D3DXVECTOR3 p4(x - gkBlockSize / 2, y + gkBlockSize / 2, z + gkBlockSize / 2);
  D3DXVECTOR3 p5(x + gkBlockSize / 2, y + gkBlockSize / 2, z + gkBlockSize / 2);
  D3DXVECTOR3 p6(x + gkBlockSize / 2, y - gkBlockSize / 2, z + gkBlockSize / 2);
  D3DXVECTOR3 p7(x - gkBlockSize / 2, y - gkBlockSize / 2, z + gkBlockSize / 2);  

  
  uint v0, v1, v2, v3;
  D3DXVECTOR3 normal;
  // Front Face
  if(!(hideFlags & HideFlags::Front))
  {
    normal = D3DXVECTOR3(0, 0, -1.0f);
    v0 = mesh->AddVertex(p0, color, normal);
    v1 = mesh->AddVertex(p1, color, normal);
    v2 = mesh->AddVertex(p2, color, normal);
    v3 = mesh->AddVertex(p3, color, normal);

    mesh->CreateTriangle(v0, v1, v2);
    mesh->CreateTriangle(v2, v3, v0);
  }
  // Back Face
  if(!(hideFlags & HideFlags::Back))
  {
    normal = D3DXVECTOR3(0, 0, 1.0f);
    v0 = mesh->AddVertex(p7, color, normal);
    v1 = mesh->AddVertex(p6, color, normal);
    v2 = mesh->AddVertex(p5, color, normal);
    v3 = mesh->AddVertex(p4, color, normal);

    mesh->CreateTriangle(v0, v1, v2);
    mesh->CreateTriangle(v2, v3, v0);
  }

  // Top Face
  if(!(hideFlags & HideFlags::Top))
  {
    normal = D3DXVECTOR3(0, 1.0f, 0);
    v0 = mesh->AddVertex(p4, color, normal);
    v1 = mesh->AddVertex(p5, color, normal);
    v2 = mesh->AddVertex(p1, color, normal);
    v3 = mesh->AddVertex(p0, color, normal);

    mesh->CreateTriangle(v0, v1, v2);
    mesh->CreateTriangle(v2, v3, v0);
  }

  // Bottom Face
  if(!(hideFlags & HideFlags::Bottom))
  {
    normal = D3DXVECTOR3(0, -1.0f, 0);
    v0 = mesh->AddVertex(p3, color, normal);
    v1 = mesh->AddVertex(p2, color, normal);
    v2 = mesh->AddVertex(p6, color, normal);
    v3 = mesh->AddVertex(p7, color, normal);
  
    mesh->CreateTriangle(v0, v1, v2);
    mesh->CreateTriangle(v2, v3, v0);
  }

  // Left Face
  if(!(hideFlags & HideFlags::Left))
  {
    normal = D3DXVECTOR3(-1.0f, 0, 0);
    v0 = mesh->AddVertex(p4, color, normal);
    v1 = mesh->AddVertex(p0, color, normal);
    v2 = mesh->AddVertex(p3, color, normal);
    v3 = mesh->AddVertex(p7, color, normal);

    mesh->CreateTriangle(v0, v1, v2);
    mesh->CreateTriangle(v2, v3, v0);
  }

  // Right Face
  if(!(hideFlags & HideFlags::Right))
  {
    normal = D3DXVECTOR3(1.0f, 0, 0);
    v0 = mesh->AddVertex(p1, color, normal);
    v1 = mesh->AddVertex(p5, color, normal);
    v2 = mesh->AddVertex(p6, color, normal);
    v3 = mesh->AddVertex(p2, color, normal);
  
    mesh->CreateTriangle(v0, v1, v2);
    mesh->CreateTriangle(v2, v3, v0);
  }
}

bool Chunk::Update(double delta)
{
  //CreateAndFillBuffers();
  return true;
}

bool Chunk::Draw()
{
  mesh->DrawMesh();
  return true;
}

D3DXVECTOR4 Chunk::GetBlockColor(const Block& block)
{
  static const D3DXVECTOR4 grassColor = D3DXVECTOR4(0.05f, 0.45f, 0.05f, 1.0f);
  static const D3DXVECTOR4 dirtColor = D3DXVECTOR4(0.15f, 0.10f, 0.10f, 1.0f);
  static const D3DXVECTOR4 rockColor = D3DXVECTOR4(0.35f, 0.35f, 0.4f, 1.0f);

  switch(block.BlockType)
  {
  case BlockTypes::Grass:
    return grassColor;
  case BlockTypes::Dirt:
    return dirtColor;
  case BlockTypes::Rock:
    return rockColor;
  default:
    assert(false);
  }
  return D3DXVECTOR4(1, 1, 1, 1);
}

Chunk::~Chunk()
{
  delete mesh;
  for(int x = 0; x < gkChunkSize; ++x)
  {
    for(int z = 0; z < gkChunkSize; ++z)
    {
      delete[] blocks[x][z];
    }
    delete[] blocks[x];
  }
  delete blocks;
}