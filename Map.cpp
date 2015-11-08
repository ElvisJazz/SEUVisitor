#include "Core_Global.h"
#include "Tile.h"
#include "Map.h"

cMap::cMap()
{
  m_Map   = NULL;
  m_Tiles = NULL;
  m_Width = m_Height = 0;
}

cMap::~cMap()
{
  Free();
}

BOOL cMap::Create(int NumLayers, int Width, int Height)
{
  // Free a prior map
  Free();

  // Save # layers, width, and height
  m_NumLayers = NumLayers;
  m_Width     = Width;
  m_Height    = Height;

  // Allocate map data memory
  if((m_Map = new int[m_NumLayers*m_Width*m_Height]) == NULL)
    return FALSE;

  // Clear it out
  ZeroMemory(m_Map, m_NumLayers*m_Width*m_Height);

  // Reset # objects to draw
 // m_NumObjectsToDraw = 0;

  return TRUE;
}

BOOL cMap::Free()
{
  // Free map array
  delete [] m_Map;
  m_Map = NULL;
  m_Width = m_Height = 0;
  m_NumLayers = 0;

  return TRUE;
}

BOOL cMap::SetMapData(int Layer, int *Data)
{
  // Error checking
  if(Layer >= m_NumLayers)
    return FALSE;

  // Copy over data
   for(int i=0; i<m_Height*m_Width; i++)
	m_Map[i] = Data[i];

  return TRUE;
}

int *cMap::GetPtr(int Layer)
{
  if(Layer >= m_NumLayers)
    return NULL;

  return &m_Map[Layer*m_Width*m_Height];
}

int cMap::GetWidth()
{
  return m_Width;
}

int cMap::GetHeight()
{
  return m_Height;
}

BOOL cMap::UseTiles(cTiles *Tiles)
{
  if((m_Tiles = Tiles) == NULL)
    return FALSE;

  return TRUE;
}

BOOL cMap::Render(int XPos,    int YPos, 
                  int NumRows, int NumColumns,
                  D3DCOLOR Color)
{
  int MapX, MapY; 
  int XOff, YOff;
  int Row, Column;
  int TileNum;
  int *MapPtr;
  int TileX, TileY;
  int Num;

  // Error checking
  if(m_Map == NULL || m_Tiles == NULL)
    return FALSE;

  // Calculate smooth scrolling variables
  MapX = XPos / m_Tiles->GetWidth(0);
  MapY = YPos / m_Tiles->GetHeight(0);
  XOff = XPos % m_Tiles->GetWidth(0);
  YOff = YPos % m_Tiles->GetHeight(0);

    // Get a pointer to the map data
    MapPtr = &m_Map[0];

    // Loop for each row and column
    for(Row=0;Row<NumRows+1;Row++) {
      for(Column=0;Column<NumColumns+1;Column++) {

        // Get the tile number to draw (and draw it)
        TileNum = MapPtr[(Row + MapY)*m_Width+Column+MapX];
		Num = TileNum / 64;
		TileX = Column * m_Tiles->GetWidth(0) - XOff;
		TileY = Row * m_Tiles->GetHeight(0) - YOff;
        m_Tiles->Draw(Num, TileNum%64, \
                      (DWORD)(1.0f*TileX),   \
                     (DWORD)(1.0f*TileY), Color,1.0f,1.0f);
      }
    }

  return TRUE;
}
