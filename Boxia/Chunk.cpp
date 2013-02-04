#include "Chunk.h"
#include <d3d10.h>
#include "Block.h"
#include <VertexFormats.h>
#include <cassert>

ID3D10InputLayout* Chunk::chunkILayout = NULL;

void Chunk::CreateChunkILayout(ZGraphics* g)
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


  Chunk::chunkILayout = g->CreateInputLayot(desc, 4);
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
      map[x][z] = (rand() % 5) * (float)((rand() % 100) / 100.0f);
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
        Material blockMat = GetBlockMaterial(blocks[x][z][y]);
        
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

        CreateBlock(worldPos.x + aX, worldPos.y + aY, worldPos.z + aZ, blockMat, hideFlags);
      }
    }
  }
  mesh->FinishMesh();

}

void Chunk::CreateBlock(float x, float y, float z, const Material& blockMat, int hideFlags)
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
    v0 = mesh->AddVertex(p0, blockMat, normal);
    v1 = mesh->AddVertex(p1, blockMat, normal);
    v2 = mesh->AddVertex(p2, blockMat, normal);
    v3 = mesh->AddVertex(p3, blockMat, normal);

    mesh->CreateTriangle(v0, v1, v2);
    mesh->CreateTriangle(v2, v3, v0);
  }
  // Back Face
  if(!(hideFlags & HideFlags::Back))
  {
    normal = D3DXVECTOR3(0, 0, 1.0f);
    v0 = mesh->AddVertex(p7, blockMat, normal);
    v1 = mesh->AddVertex(p6, blockMat, normal);
    v2 = mesh->AddVertex(p5, blockMat, normal);
    v3 = mesh->AddVertex(p4, blockMat, normal);

    mesh->CreateTriangle(v0, v1, v2);
    mesh->CreateTriangle(v2, v3, v0);
  }

  // Top Face
  if(!(hideFlags & HideFlags::Top))
  {
    normal = D3DXVECTOR3(0, 1.0f, 0);
    v0 = mesh->AddVertex(p4, blockMat, normal);
    v1 = mesh->AddVertex(p5, blockMat, normal);
    v2 = mesh->AddVertex(p1, blockMat, normal);
    v3 = mesh->AddVertex(p0, blockMat, normal);

    mesh->CreateTriangle(v0, v1, v2);
    mesh->CreateTriangle(v2, v3, v0);
  }

  // Bottom Face
  if(!(hideFlags & HideFlags::Bottom))
  {
    normal = D3DXVECTOR3(0, -1.0f, 0);
    v0 = mesh->AddVertex(p3, blockMat, normal);
    v1 = mesh->AddVertex(p2, blockMat, normal);
    v2 = mesh->AddVertex(p6, blockMat, normal);
    v3 = mesh->AddVertex(p7, blockMat, normal);
  
    mesh->CreateTriangle(v0, v1, v2);
    mesh->CreateTriangle(v2, v3, v0);
  }

  // Left Face
  if(!(hideFlags & HideFlags::Left))
  {
    normal = D3DXVECTOR3(-1.0f, 0, 0);
    v0 = mesh->AddVertex(p4, blockMat, normal);
    v1 = mesh->AddVertex(p0, blockMat, normal);
    v2 = mesh->AddVertex(p3, blockMat, normal);
    v3 = mesh->AddVertex(p7, blockMat, normal);

    mesh->CreateTriangle(v0, v1, v2);
    mesh->CreateTriangle(v2, v3, v0);
  }

  // Right Face
  if(!(hideFlags & HideFlags::Right))
  {
    normal = D3DXVECTOR3(1.0f, 0, 0);
    v0 = mesh->AddVertex(p1, blockMat, normal);
    v1 = mesh->AddVertex(p5, blockMat, normal);
    v2 = mesh->AddVertex(p6, blockMat, normal);
    v3 = mesh->AddVertex(p2, blockMat, normal);
  
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

const Material& Chunk::GetBlockMaterial(const Block& block)
{
  static const Material grassMat = { 
    D3DXCOLOR(0.25f, 0.55f, 0.25f, 1.0f), D3DXCOLOR(0.25f, 0.25f, 0.25f, 5.0f)
  };

  static const Material dirtMat = { 
    D3DXCOLOR(0.20f, 0.12f, 0.12f, 1.0f), D3DXCOLOR(0.15f, 0.10f, 0.05f, 1.0f)
  };

  static const Material rockMat = { 
    D3DXCOLOR(0.25f, 0.25f, 0.25f, 1.0f), D3DXCOLOR(0.45f, 0.45f, 0.45f, 35.0f)
  };

  switch(block.BlockType)
  {
  case BlockTypes::Grass:
    return grassMat;
  case BlockTypes::Dirt:
    return dirtMat;
  case BlockTypes::Rock:
    return rockMat;
  default:
    assert(false);
  }
  return rockMat;
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