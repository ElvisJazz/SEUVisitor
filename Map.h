#ifndef _MAP_H_
#define _MAP_H_

class cMap
{
  private:
    int    m_Width, m_Height;  // Width and height of map
    int    m_NumLayers;        // # of layers
    int   *m_Map;              // array for tile information
    cTiles *m_Tiles;            // cTile class to use for tiles

  public:
    cMap();   // Constructor
    ~cMap();  // Destructor

    // Function to create and free a map class
    BOOL Create(int NumLayers, int Width, int Height);
    BOOL Free();

    // Function to set a map's layer data 
    BOOL SetMapData(int Layer, int *Data);

    int *GetPtr(int Layer); // Get pointer to map array
    int GetWidth();   // Get width of map
    int GetHeight();  // Get height of map

    // Assign cTile class object to use for drawing map tiles
    BOOL UseTiles(cTiles *Tiles);

    // Render map using specified top-left map coordinates, as
    // well as # of columns and rows to draw, plus layer used
    // to draw objects.
    BOOL Render(int XPos, int YPos,                         \
                int NumRows, int NumColumns,                \
                D3DCOLOR Color);
};

#endif
