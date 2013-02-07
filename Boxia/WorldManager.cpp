#include "WorldManager.h"
#include "Chunk.h"
#include "ChunkManager.h"
#include <d3d10.h>
#include <LibZerkish.h>

WorldManager::WorldManager(ZGraphics* graphics)
{
  this->graphics = graphics;
  chunkManager = nullptr;
  blockLayout = nullptr;
}

WorldManager::~WorldManager()
{
  delete chunkManager;
  COMRELEASE(blockLayout);
}

bool WorldManager::Initialize(ID3D10InputLayout* blockLayout)
{
  this->blockLayout = blockLayout;
  blockLayout->AddRef();
  chunkManager = new ChunkManager();
  return true;
}


void WorldManager::FillWorld()
{
  chunks.push_back(new Chunk());
  chunks[0]->Init();
  chunks[0]->SetWorldPosition(0.0f, 0, 0);
  chunks.push_back(new Chunk());
  chunks[1]->Init();
  chunks[1]->SetWorldPosition(1.0f, 0, 0);
  chunks.push_back(new Chunk());
  chunks[2]->Init();
  chunks[2]->SetWorldPosition(-1.0f, 0, 0);

  //for(int i = 0; i < 64; ++i)
  //{
  //  chunks.push_back(new Chunk());
  //  chunks[i]->Init();
  //  chunks[i]->SetWorldPosition(
  //    static_cast<float>(((i % 8) - 4) * gkChunkSize),
  //    0.0f,
  //    static_cast<float>(((i / 8) - 4) * gkChunkSize)
  //  );
  //}
}

void WorldManager::UpdateWorld(double delta)
{
  for(Chunk* chunk : chunks)
  {
    if(chunk && chunk->NeedsRebuild())
    {
      chunk->RebuildChunk(graphics);
    }
  }
}

void WorldManager::DrawWorld()
{
  
  graphics->SetInputLayout(blockLayout);
  for(Chunk* chunk : chunks)
  {
    if(chunk)
    {
      chunk->Draw(graphics);
    }
  }
}

void WorldManager::UnloadChunk(Chunk** chunk, int num)
{
  chunkManager->AsyncChunkUnload(chunk, num);
  *chunk = NULL;
}

Chunk* WorldManager::LoadChunk(int chunkX, int chunkY, int chunkZ)
{
  return NULL;
}