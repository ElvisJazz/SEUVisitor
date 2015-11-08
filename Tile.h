#ifndef _TILE_H_
#define _TILE_H_

class cTiles
{
  private:
    cGraphics *m_Graphics;     // Parent cGraphics

    int       m_NumTextures;  // # of textures
    cTexture  *m_Textures;     // cTexture array
    int      *m_Widths;       // Tile widths array
    int      *m_Heights;      // Tile heights array
    int      *m_Columns;      // # columns in texture

  public:
    cTiles();
    ~cTiles();

    // Functions to create and free the tile interface
    BOOL Create(cGraphics *Graphics, int NumTextures);
    BOOL Free();

    // Functions to load and free a single texture
    BOOL Load(int TextureNum, char *Filename,                \
              short TileWidth = 0, short TileHeight = 0,      \
              D3DCOLOR Transparent = 0,                       \
              D3DFORMAT Format = D3DFMT_A8R8G8B8);
    BOOL Free(int TextureNum);

    // Functions to retrieve tile dimensions and 
    // # of tiles in a texture.
    int GetWidth(int TextureNum);
    int GetHeight(int TextureNum);
    int GetNum(int TextureNum);

    // Enable or disable transparent blitting
    BOOL SetTransparent(BOOL Enabled = TRUE);

    // Draw a single tile to location
    BOOL Draw(int TextureNum, int TileNum,                  \
              int ScreenX, int ScreenY,                     \
              D3DCOLOR Color = 0xFFFFFFFF,                    \
              float XScale = 1.0f, float YScale = 1.0f);
};

#endif
