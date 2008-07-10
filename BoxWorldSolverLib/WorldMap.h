#ifndef __BW_WORLDMAP__
#define __BW_WORLDMAP__

#include <string>
#include "BWSolverException.h"


#define WM_MAXX 20
#define WM_MAXY 20


struct Tile
{
    enum Enum
    {
        Empty, Wall, BoxTarget, Player, Box
    };
};


class WorldMap
{
public:
    WorldMap();
    WorldMap(int x, int y);
    WorldMap(const WorldMap &);
    ~WorldMap();

    /// Set dimensions
    void SetXY(int x,int y);

    /// Clear all map content
    void Clear();

    /// Clear map content for particular layer
    void Clear(int layer);

    /// copy content of map to this map
    void Copy(const WorldMap &);

    /// draw the world boundaries
    void DrawBoundaries();

    /// set a tile at XY position
    void SetTile(int layer, int x,int y, Tile::Enum);

    /// gets a tile at XY position
    Tile::Enum GetTile(int layer, int x,int y) const;

    /// returns true if position contains a wall or box
    bool IsSolidTile(short x, short y) const;

    int GetX() const;
    int GetY() const;
    int CountNumberOfBoxes() const;

    // verify the validity of boxworld world
    void Verify() const;

    void SaveToFile  (std::string filename);
    void LoadFromFile(std::string filename);
    void Print();

private:
    int mSizeX,mSizeY;

    Tile::Enum mMap[3][WM_MAXX][WM_MAXY];

    void tile2char(Tile::Enum t0, Tile::Enum t1, char &ch);
    void char2tile(char ch, Tile::Enum &t0, Tile::Enum &t1);
};



#endif