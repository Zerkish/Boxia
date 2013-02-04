#include "WorldManager.h"
#include "Chunk.h"
#include "ChunkManager.h"

WorldManager::WorldManager()
{
  chunkManager = new ChunkManager();
}

void WorldManager::UnloadChunk(Chunk* chunk)
{

}

Chunk* WorldManager::LoadChunk(int chunkX, int chunkY, int chunkZ)
{

}