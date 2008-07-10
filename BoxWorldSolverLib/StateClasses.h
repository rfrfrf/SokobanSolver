#ifndef __BW_STATE__
#define __BW_STATE__

#include <set>
#include "WorldMap.h"

// the memory size used by the player class should be small
class Player
{
public:
    char x,y;
};

// the memory size used by the box class should be small
class Box
{
public:
    char x,y;
    unsigned char f; // f = move freedom flags
};


// this is currently hardcoded
#define STATE_MAX_BOXES 20


class State;

/// The Storestate class is a condensed state of a boxworld level
/// Instances of the storestate class will be stored for lookups
///
class StoreState
{
    friend class State;
public:  
    StoreState();
    StoreState(int maxboxes);
    virtual ~StoreState();

    /// copy content of storestate to this storestate
    void Copy(const StoreState &, int NumberBoxes);

    /// calculate a hash value using the box positions
    /// unfortunately boxes with positions swopped will result in a different hash value!
    long HashValue(short NBoxes);

    /// access player position
    void SetPlayerXY(int x, int y);
    void GetPlayerXY(int &x, int &y);

    /// used for keeping track of solution depth
    unsigned char depth;
protected:
    Box boxes[STATE_MAX_BOXES]; // staticly allocated memory
    Player player;
    /// the number of boxes is intentionally not stored in this class in order to save space 
};

/// The State class is an elaborated state of a boxworld level
/// The Boxworld world is stored as a matrix on which indexed lookups are done.
/// This class contains tests for identifying "dead-end" (non-legal) states
///
class State : public StoreState
{
public:
    State();
    State(const WorldMap & map);
    virtual ~State();

    /// Copies the worldmap to this state
    void Copy(const WorldMap & map);

    /// Copies the content of state to this state
    void Copy(const State & state);

    long HashValue();

    /// Sets the freedom bits on the boxes
    /// Freedom bits indicates the directions into which the player can push a box
    void SetBoxFreedom();

    /// move box n to position nx, ny
    void MoveBox(short n, short nx, short ny);

    /// returns true if state is not a "dead-end"
    bool LegalState(int movesLeft);

    /// returns true if all boxes are on box targets
    bool CheckWin();

    /// Generates the next state from this state using the "Box Freedom bits"
    /// returns false if no further state can be generated from this state
    bool CreateNextState(State & nextstate);

    int GetNumberOfBoxes();

    /// Determines if states are equal
    /// "PlayerAccessFloodFill" must be called before calling the "Equals" function
    bool Equals(const StoreState & storestate);

    WorldMap & GetWorldMap();

protected:
    int NumberBoxes;
    WorldMap mWorldMap;

    /// Flood fills the area in which the character can move without pushing any boxes
    void PlayerAccessFloodFill(short x, short y);

    /// "Dead-end" Test1
    /// Corner test identifies whether a box at position X, Y is in a corner.
    inline bool CornerTest(short x, short y);
    inline bool TestDirection(short &x, short &y, short dx, short dy);
    inline bool RecurseCorner(short &x, short &y,short rx, short ry);

    /// "Dead-end" Test2
    /// Cramp test identifies boxes that are pushed against a wall and can never be
    /// pushed onto a box target or pushed away from the wall.
    inline bool CrampTest(short x, short y);
    bool Search(short x, short y, short cdx, short cdy, short dx, short dy);
};




#endif