#include "WorldMap.h"
#include <iostream>
#include <fstream>

/*void WorldMap::tile2char(Tile::Enum t0, Tile::Enum t1, char &ch)
{
ch = ' '; 
if (t0 == Tile::BoxTarget)
{
switch(t1)
{
case Tile::Empty: ch = 'X';break;
case Tile::Wall: ch = '?';break;
case Tile::Box: ch = 'B';break;
case Tile::Player: ch = 'M';break;
}
}
else
{
switch(t1)
{
case Tile::Empty: ch = ' ';break;
case Tile::Wall: ch = 'W';break;
case Tile::Box: ch = 'b';break;
case Tile::Player: ch = 'm';break;
}
}
}*/

void WorldMap::Print()
{
    for (int y=0;y<GetY();y++)
    {
        for (int x=0;x<GetX();x++)
        { 
            char ch;
            Tile::Enum tx = GetTile(1, x, y);

            tile2char(GetTile(0, x, y),
                tx,
                ch);
            std::cerr<<ch;
        }
        std::cerr<<std::endl;
    }
}


WorldMap::WorldMap()
{
    SetXY(WM_MAXX, WM_MAXY);
}

WorldMap::WorldMap(int x, int y)
{
    SetXY(x,y);
}

WorldMap::WorldMap(const WorldMap & map)
{
    SetXY(map.GetX(),map.GetY());
    Copy(map);
}

void WorldMap::SetXY(int x,int y)
{
    mSizeX = x;
    mSizeY = y;
}

WorldMap::~WorldMap()
{
}

void WorldMap::Clear()
{
    for (int k=0;k<3;k++)
    {
        Clear(k);
    }
}

void WorldMap::DrawBoundaries()
{
    for (int rx=0;rx<mSizeX;rx++)
    {
        mMap[1][rx][0] = Tile::Wall;
        mMap[1][rx][mSizeY-1] = Tile::Wall;
    }
    for (int ry=1;ry<mSizeY-1;ry++)
    {
        mMap[1][0][ry] = Tile::Wall;
        mMap[1][mSizeX-1][ry] = Tile::Wall;
    }
}

void WorldMap::Clear(int layer)
{
    for (int x=0;x<mSizeX;x++)
    {
        for (int y=0;y<mSizeY;y++)
        {
            mMap[layer][x][y] = Tile::Empty;
        }
    }
}

void WorldMap::Copy(const WorldMap & map)
{
    memcpy((void*)mMap, (void *)map.mMap, 3*WM_MAXX*WM_MAXY*sizeof(Tile::Enum));   
}

int WorldMap::CountNumberOfBoxes() const
{
    int Boxes = 0;
    for (int x=0;x<mSizeX;x++)
    {
        for (int y=0;y<mSizeY;y++)
        {
            if (mMap[1][x][y] == Tile::Box)
            {
                Boxes ++;
            }
        }
    }
    return Boxes;
}

void WorldMap::Verify() const
{
    int Boxes = 0;
    int BoxTargets = 0;
    int Players = 0;
    for (int x=0;x<mSizeX;x++)
    {
        for (int y=0;y<mSizeY;y++)
        {
            switch (mMap[1][x][y])
            {
            case Tile::Player: Players++;break;
            case Tile::Box: Boxes++; break;
            default:;
            }
            switch (mMap[0][x][y])
            {
            case Tile::BoxTarget: BoxTargets++; break;
            default:;
            }
        }
    }
    if (Boxes>20)
    {
        throw BWSolverException("Maximum number of 20 boxes exceeded!");
    }
    if (Boxes!=BoxTargets)
    {
        throw BWSolverException("Number of box targets mismatches the number of boxes.");
    }
    if (Boxes==0)
    {
        throw BWSolverException("No boxes in the area.");
    }
    if (Players==0)
    {
        throw BWSolverException("No player in the area.");
    }
    if (Players>1)
    {
        throw BWSolverException("More than one player in the area.");
    }
}

bool WorldMap::IsSolidTile(short x, short y) const
{
    switch (mMap[1][x][y])
    {
    case Tile::Wall:
    case Tile::Box:
        return true;
    }
    return false;
}


void WorldMap::SetTile(int layer, int x,int y, Tile::Enum t)
{
    mMap[layer][x][y] = t;
}

Tile::Enum WorldMap::GetTile(int layer, int x,int y) const
{
    return mMap[layer][x][y];
}


int WorldMap::GetX() const
{
    return mSizeX;
}

int WorldMap::GetY() const
{
    return mSizeY;
}


void WorldMap::SaveToFile  (std::string filename)
{
    char ch;
    std::ofstream file(filename.c_str(), std::ios::out);

    file<<GetX()<<std::endl;
    file<<GetY()<<std::endl;

    for (int y=0;y<GetY();y++)
    {
        for (int x=0;x<GetX();x++)
        {
            tile2char(GetTile(0, x, y), GetTile(1, x, y), ch);
            file<<ch;
        }
        file<<std::endl;
    }

    file.close();
}

void WorldMap::LoadFromFile(std::string filename)
{
    std::ifstream file(filename.c_str(), std::ios::in);

    int x=0;
    int y=0;
    char ch;
    Tile::Enum t0,t1;

    int mx,my;
    file>>mx;
    file>>my;
    SetXY(mx,my);

    if (mx>WM_MAXX) mx=WM_MAXX;
    if (my>WM_MAXY) my=WM_MAXY;

    char buff[50];
    file.getline(buff,50); // get rid of endl
    while (!file.eof())
    {
        file.get(ch);
        char2tile(ch, t0, t1);
        SetTile(0, x, y, t0);
        SetTile(1, x, y, t1);
        x++;
        if (ch==10) 
        {
            x=0;
            y++;
        }
    }

    file.close();
}



void WorldMap::char2tile(char ch, Tile::Enum &t0, Tile::Enum &t1)
{
    t0 = Tile::Empty; t1 = Tile::Empty;

    switch (ch)
    {
    case ' ': t0 = Tile::Empty; t1 = Tile::Empty; break;
    case 'W': t0 = Tile::Empty; t1 = Tile::Wall; break;
    case 'X': t0 = Tile::BoxTarget; t1 = Tile::Empty; break;
    case 'b': t0 = Tile::Empty; t1 = Tile::Box; break;
    case 'B': t0 = Tile::BoxTarget; t1 = Tile::Box; break;
    case 'm': t0 = Tile::Empty; t1 = Tile::Player; break;
    case 'M': t0 = Tile::BoxTarget; t1 = Tile::Player; break;
    }
}

void WorldMap::tile2char(Tile::Enum t0, Tile::Enum t1, char &ch)
{
    ch = ' ';
    if (t0 == Tile::BoxTarget)
    {
        switch(t1)
        {
        case Tile::Empty: ch = 'X';break;
        case Tile::Wall: ch = '?';break;
        case Tile::Box: ch = 'B';break;
        case Tile::Player: ch = 'M';break;
        }
    }
    else
    {
        switch(t1)
        {
        case Tile::Empty: ch = ' ';break;
        case Tile::Wall: ch = 'W';break;
        case Tile::Box: ch = 'b';break;
        case Tile::Player: ch = 'm';break;
        }
    }
}



