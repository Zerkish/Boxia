#ifndef CHUNK_H
#define CHUNK_H
#include <d3d10.h>
#include <D3DX10.h>
#include <LibZerkish.h>

struct Block;

const int gkChunkSize = 16;

class Chunk
{
public:
  friend class ChunkManager;

  // Ctor
  Chunk(ZGraphics* graphics);
  bool Init();

  const D3DXVECTOR3& WorldPosition() const;
  void SetWorldPosition(float x, float y, float z);

  bool Update(double delta);
  bool Draw();

  ~Chunk();
private:
  static ID3D10InputLayout* chunkILayout;
  static void CreateChunkILayout(ZGraphics* g);
  
  D3DXVECTOR3 worldPos;
  void CreateAndFillBuffers();
  
  const Material& GetBlockMaterial(const Block& block);
  void CreateBlock(float f, float y, float z, const Material& blockMat, int hideFlags = 0);
  
  enum HideFlags : int
  {
    Left = 1,
    Right = 2,
    Front = 4,
    Back = 8,
    Bottom = 16,
    Top = 32,
  };

  Block*** blocks;
  ZGraphics* graphics;
  ZMesh* mesh;
};

// INLINES
inline const D3DXVECTOR3& Chunk::WorldPosition() const { return worldPos; }
inline void Chunk::SetWorldPosition(float x, float y, float z)
{
  worldPos.x = x;
  worldPos.y = y;
  worldPos.z = z;
  if(mesh) mesh->SetPosition(x, y, z);
}

#endif