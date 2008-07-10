#include "StateClasses.h"
#include "WorldMap.h"


StoreState::StoreState()
{
}

StoreState::~StoreState()
{
}

void StoreState::SetPlayerXY(int x, int y)
{
    player.x = x;
    player.y = y;
}

void StoreState::GetPlayerXY(int &x, int &y)
{
    x = player.x;
    y = player.y;
}

void StoreState::Copy(const StoreState & state, int NumberBoxes)
{
    player = state.player;
    for (int j=0;j<NumberBoxes;j++)
    {
        boxes[j] = state.boxes[j];
    }
}


long StoreState::HashValue(short NBoxes)
{
    long val=0;
    for (int j=0;j<NBoxes;j++)
    {
        val = (val*16+boxes[j].x-1) % 10000000;
        val = (val*18+boxes[j].y-1) % 10000000;
    }
    return val;
}

State::State() : StoreState()
{
}

State::State(const WorldMap & map) : StoreState()
{
    Copy(map);
}

State::~State()
{
}


void State::Copy(const WorldMap & map)
{
    mWorldMap.Copy(map);
    NumberBoxes = mWorldMap.CountNumberOfBoxes();
    int boxn = 0;
    for (int x=0;x<mWorldMap.GetX();x++)
    {
        for (int y=0;y<mWorldMap.GetY();y++)
        {
            if (mWorldMap.GetTile(1,x,y) == Tile::Box)
            {
                boxes[boxn].x = x;
                boxes[boxn].y = y;
                boxn++;
            }
            if (mWorldMap.GetTile(1,x,y) == Tile::Player)
            {
                player.x = x;
                player.y = y;
            }
        }
    }
}


void State::Copy(const State & state)
{
    player = state.player;
    NumberBoxes = state.NumberBoxes;
    for (int j=0;j<state.NumberBoxes;j++)
    {
        boxes[j] = state.boxes[j];
    }
    mWorldMap.Copy(state.mWorldMap);
}

bool State::Equals(const StoreState & storestate)
{
    if (mWorldMap.GetTile(2, storestate.player.x, storestate.player.y) != Tile::Player)
    {
        return false;
    }
    for (int j=0;j<NumberBoxes;j++)
    {
        if ((boxes[j].x != storestate.boxes[j].x)||
            (boxes[j].y != storestate.boxes[j].y))
        {
            return false;
        }
    }
    return true;
}

long State::HashValue()
{
    return StoreState::HashValue(NumberBoxes);
}

int State::GetNumberOfBoxes()
{
    return NumberBoxes;
}

void State::PlayerAccessFloodFill(short x, short y)
{
    mWorldMap.SetTile(2, x, y, Tile::Player);
    if ((mWorldMap.GetTile(2, x-1, y) != Tile::Player)&&(!mWorldMap.IsSolidTile(x-1, y))) PlayerAccessFloodFill(x-1, y);
    if ((mWorldMap.GetTile(2, x, y-1) != Tile::Player)&&(!mWorldMap.IsSolidTile(x, y-1))) PlayerAccessFloodFill(x, y-1);
    if ((mWorldMap.GetTile(2, x, y+1) != Tile::Player)&&(!mWorldMap.IsSolidTile(x, y+1))) PlayerAccessFloodFill(x, y+1);
    if ((mWorldMap.GetTile(2, x+1, y) != Tile::Player)&&(!mWorldMap.IsSolidTile(x+1, y))) PlayerAccessFloodFill(x+1, y);
}



void State::SetBoxFreedom()
{
    mWorldMap.Clear(2);
    PlayerAccessFloodFill(player.x, player.y);
    for (int j=0;j<NumberBoxes;j++)
    {
        boxes[j].f = 0; // reset freedom byte
        if ((mWorldMap.GetTile(2, boxes[j].x-1, boxes[j].y) == Tile::Player)&&
            (!mWorldMap.IsSolidTile(boxes[j].x+1, boxes[j].y)))
            boxes[j].f|=1;
        if ((mWorldMap.GetTile(2, boxes[j].x,   boxes[j].y+1) == Tile::Player)&&
            (!mWorldMap.IsSolidTile(boxes[j].x,   boxes[j].y-1)))
            boxes[j].f|=2;
        if ((mWorldMap.GetTile(2, boxes[j].x+1, boxes[j].y) == Tile::Player)&&
            (!mWorldMap.IsSolidTile(boxes[j].x-1, boxes[j].y))) 
            boxes[j].f|=4;
        if ((mWorldMap.GetTile(2, boxes[j].x,   boxes[j].y-1) == Tile::Player)&&
            (!mWorldMap.IsSolidTile(boxes[j].x,   boxes[j].y+1))) 
            boxes[j].f|=8;
    }
}

void State::MoveBox(short n, short nx, short ny)
{
    player.x = boxes[n].x;
    player.y = boxes[n].y;
    mWorldMap.SetTile(1, boxes[n].x, boxes[n].y, Tile::Empty);
    boxes[n].x = (char) nx; 
    boxes[n].y = (char) ny;
    mWorldMap.SetTile(1, boxes[n].x, boxes[n].y, Tile::Box);
}


bool State::LegalState(int movesLeft)
{
    //return true;
    int boxesNotOnPositions = 0;
    for (int n=0;n<NumberBoxes;n++)
    {
        if (mWorldMap.GetTile(0, boxes[n].x, boxes[n].y) != Tile::BoxTarget) boxesNotOnPositions++;
    }
    if (boxesNotOnPositions>movesLeft) return false;

    for (int j=0;j<NumberBoxes;j++)
    {
        if (mWorldMap.GetTile(0, boxes[j].x, boxes[j].y)!=Tile::BoxTarget)
        {
            if (CornerTest(boxes[j].x, boxes[j].y))
            {
                return false;
            }
        }
        if (CrampTest(boxes[j].x, boxes[j].y)) 
        {
            return false; // failed! this is a dead-end state!
        }
    }
    return true;
}

/// "Dead-end" Test1
/// Corner test identifies whether a box at position X, Y is in a corner.
/// A box in a corner can NEVER be moved by the player.
///
/// A corner may be created by other boxes
/// 
///  bb       
///  bb
///    
/// In this case the algorithm will apply the corner test recursively by making boxes that are potentially 
/// cornered by other boxes walls (in essence they represent walls, since they are immovable until other boxes
/// can be moved)
///
///  bb --> bW --> WW 
///  bW     bW     bW
///
/// This algorithm will therefore identify a box in a corner, thus a "Dead-end"
/// 
inline bool State::CornerTest(short x, short y)
{
    return ((TestDirection(x, y, x-1, y-1))||(TestDirection(x, y, x+1, y-1))||
        (TestDirection(x, y, x-1, y+1))||(TestDirection(x, y, x+1, y+1)));
}

inline bool State::TestDirection(short &x, short &y, short dx, short dy)
{
    bool b;
    if (mWorldMap.IsSolidTile(dx, y) && (mWorldMap.IsSolidTile(x, dy)))
    {
        b = true;
        if (mWorldMap.GetTile(1,dx,y) == Tile::Box) 
            b = RecurseCorner(x, y, dx, y);

        if (mWorldMap.GetTile(1,x,dy) == Tile::Box) 
            b = b && RecurseCorner(x, y, x, dy);
    } 
    else 
    {
        b = false;
    }
    return b;
}


inline bool State::RecurseCorner(short &x, short &y, short rx, short ry)
{
    Tile::Enum t0 = mWorldMap.GetTile(1,x,y);
    mWorldMap.SetTile(1,x,y,Tile::Wall);
    bool result = CornerTest(rx, ry);
    mWorldMap.SetTile(1,x,y,t0);
    return result;
}

// Search is used by CramptTest
inline bool State::Search(short x, short y, short cdx, short cdy, short dx, short dy)
{
    while (mWorldMap.GetTile(1,x,y)!=Tile::Wall)
    {
        x = x + dx;
        y = y + dy;
        if ((mWorldMap.GetTile(0,x,y)==Tile::BoxTarget)|| (mWorldMap.GetTile(1,x+cdx,y+cdy)!=Tile::Wall))
        {
            return false;
        }
    }
    return true;
}

/// "Dead-end" Test2
/// Cramp test identifies boxes that are pushed against a wall and can never be
/// pushed onto a box target or pushed away from the wall.
bool State::CrampTest(short x, short y)
{
    if (mWorldMap.GetTile(0,x,y)==Tile::BoxTarget)
    {
        return false;
    }

    if (mWorldMap.GetTile(1,x,y-1)==Tile::Wall)
        if ((Search(x, y, 0, -1, 1, 0) && Search(x, y, 0, -1, -1, 0)))
            return true;

    if (mWorldMap.GetTile(1,x,y+1)==Tile::Wall)
        if ((Search(x, y, 0, 1, 1, 0) && Search(x, y, 0, 1, -1, 0)))
            return true;

    if (mWorldMap.GetTile(1,x+1,y)==Tile::Wall)
        if ((Search(x, y, 1, 0, 0, 1) && Search(x, y, 1, 0, 0, -1)))
            return true;

    if (mWorldMap.GetTile(1,x-1,y)==Tile::Wall)
        if ((Search(x, y, -1, 0, 0, 1) && Search(x, y, -1, 0, 0, -1)))
            return true;

    return false;
}


bool State::CheckWin()
{
    for (int j=0;j<NumberBoxes;j++)
    {
        if (mWorldMap.GetTile(0, boxes[j].x, boxes[j].y) != Tile::BoxTarget)
        {
            return false;
        }
    }
    return true;
}

bool State::CreateNextState(State & nextstate)
{
    bool StateCreated = false;

    for (int j=0;j<NumberBoxes;j++)
    {
        if ((boxes[j].f & 1) != 0)
        {
            boxes[j].f ^= 1;
            nextstate.Copy(*this);
            nextstate.MoveBox(j,boxes[j].x + 1, boxes[j].y);
            StateCreated = true;
            break;
        }
        if ((boxes[j].f & 2) != 0)
        {
            boxes[j].f ^= 2;
            nextstate.Copy(*this);
            nextstate.MoveBox(j,boxes[j].x, boxes[j].y - 1);
            StateCreated = true;
            break;
        }
        if ((boxes[j].f & 4) != 0)
        {
            boxes[j].f ^= 4;
            nextstate.Copy(*this);
            nextstate.MoveBox(j,boxes[j].x - 1, boxes[j].y);
            StateCreated = true;
            break;
        }
        if ((boxes[j].f & 8) != 0)
        {
            boxes[j].f ^= 8;
            nextstate.Copy(*this);
            nextstate.MoveBox(j,boxes[j].x, boxes[j].y + 1);
            StateCreated = true;
            break;
        }
    }

    return StateCreated;
}


WorldMap & State::GetWorldMap()
{
    return mWorldMap;
}