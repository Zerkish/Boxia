#ifndef WORLDMANAGER_H
#define WORLDMANAGER_H

#include <vector>
class Chunk;
class ChunkManager;

using std::vector;

class WorldManager
{
public:
  WorldManager();

  // Fills the world up to MAX_CHUNKS
  void FillWorld();

  void UnloadChunk(Chunk* chunk);
  Chunk* LoadChunk(int chunkX, int chunkY, int chunkZ);
  
  static const int MAX_CHUNKS = 16;
private:
  vector<Chunk*> chunks;
  ChunkManager* chunkManager;
};

#endif