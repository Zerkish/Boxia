#ifndef CHUNK_H
#define CHUNK_H
#include <d3d10.h>
#include <D3DX10.h>
#include <LibZerkish.h>

struct Block;

const int gkChunkSize = 1;

class Chunk
{
public:
  friend class ChunkManager;

  // Constructor
  Chunk();

  // Destructor
   ~Chunk();

  bool Init();

  void RebuildChunk(ZGraphics* graphics);

  bool Update(double delta);
  bool Draw(ZGraphics* graphics);
#pragma region Accessors
  const D3DXVECTOR3& WorldPosition() const;
  void SetWorldPosition(float x, float y, float z);

  bool NeedsRebuild() const;
#pragma endregion

private:
  bool needsRebuild;
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

inline bool Chunk::NeedsRebuild() const { return needsRebuild; }

#endif