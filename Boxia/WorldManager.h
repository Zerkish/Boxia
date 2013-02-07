#ifndef WORLDMANAGER_H
#define WORLDMANAGER_H

#include <vector>
class Chunk;
class ChunkManager;
class ZGraphics;

struct ID3D10InputLayout;

using std::vector;


class WorldManager
{
public:
  WorldManager(ZGraphics* graphics);
  ~WorldManager();

  bool Initialize(ID3D10InputLayout* blockLayout);

  // Fills the world up to MAX_CHUNKS
  void FillWorld();

  void UnloadChunk(Chunk** chunk, int num);
  Chunk* LoadChunk(int chunkX, int chunkY, int chunkZ);

  // Performs updates and detects cluster events.
  void UpdateWorld(double delta);
  
  void DrawWorld();
  
  static const int MAX_CHUNKS = 16;
private:
  ID3D10InputLayout* blockLayout;
  vector<Chunk*> chunks;
  ChunkManager* chunkManager;
  ZGraphics* graphics;
};

#endif