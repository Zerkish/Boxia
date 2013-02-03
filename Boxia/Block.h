#ifndef BLOCK_H
#define BLOCK_H

const float gkBlockSize = 1.0f;

typedef unsigned char ubyte;

enum BlockTypes : ubyte
{
  Undefined = 0,

  Grass = 1,
  Dirt = 2,
  Rock = 3,
};



struct Block
{
public:
  Block();

  inline bool Visible() { return visible; }
  inline void SetVisible(bool value) { visible = value; }

  ~Block();
  BlockTypes BlockType;
  
private:
  bool visible;
};


#endif