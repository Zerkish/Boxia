#ifndef WORLDMANAGER_H
#define WORLDMANAGER_H

#include <vector>
class Chunk;
class ChunkManager;
class ZGraphics;

using std::vector;


class WorldManager
{
public:
  WorldManager();

  // Fills the world up to MAX_CHUNKS
  void FillWorld(ZGraphics* graphics);

  void UnloadChunk(Chunk** chunk, int num);
  Chunk* LoadChunk(int chunkX, int chunkY, int chunkZ);

  void DrawWorld();
  
  static const int MAX_CHUNKS = 16;
private:
  vector<Chunk*> chunks;
  ChunkManager* chunkManager;
  ZGraphics* graphics;
};

#endif