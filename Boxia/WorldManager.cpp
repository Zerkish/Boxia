#include "WorldManager.h"
#include "Chunk.h"
#include "ChunkManager.h"

WorldManager::WorldManager()
{
  chunkManager = new ChunkManager();
}

void WorldManager::FillWorld(ZGraphics* graphics)
{
  for(int i = 0; i < 64; ++i)
  {
    chunks.push_back(new Chunk(graphics));
    chunks[i]->Init();
    chunks[i]->SetWorldPosition(
      ((i % 8) - 4) * 16,
      0,
      ((i / 8) - 4) * 16);
  }
}

void WorldManager::DrawWorld()
{
  for(Chunk* chunk : chunks)
  {
    if(chunk)
    {
      chunk->Draw();
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