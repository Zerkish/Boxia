#ifndef CHUNKMANAGER_H
#define CHUNKMANAGER_H

#include <stack>
#include <fstream>
#include <thread>

class Chunk;

class ChunkManager
{
public:
  ChunkManager();

  Chunk* GetLoadedCluster();

  void UnloadChunk(Chunk* chunk);
  Chunk* LoadChunk(int chunkX, int chunkY, int chunkZ);

private:
  std::stack<Chunk*> loadStack;
  std::stack<Chunk*> unloadStack;
  std::thread loadThread;
  std::thread unloadThread;
  std::fstream readStream;
  std::fstream writeStream;
};

#endif