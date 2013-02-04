#include "ChunkManager.h"
#include "Chunk.h"
#include <string>
#include <sstream>
#include <iostream>

ChunkManager::ChunkManager()
{

}

void ChunkManager::UnloadChunk(Chunk* chunk)
{

}

Chunk* ChunkManager::LoadChunk(int chunkX, int chunkY, int chunkZ)
{
  std::stringstream strStream;
  strStream << chunkX << "_" << chunkY << "_" << chunkZ << ".dat";


  readStream.open(strStream.str().c_str(), std::ios::binary);
  if(!readStream.is_open())
  {
    return nullptr;
  }

  char buffer[4096];
  while(readStream.readsome(buffer, 4096))
  {

  }
  
}