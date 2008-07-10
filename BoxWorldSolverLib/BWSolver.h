#ifndef __BW_SOLVER__
#define __BW_SOLVER__

#pragma warning(disable:4786)

#include <iostream>
#include "WorldMap.h"
#include "BWSolverException.h"
#include "StateClasses.h"
#include "BWSolverObserver.h"
#include <list>
#include <map>

class WorldMap;
class StoreStateMemoryBlock;

class BWSolver
{
public:
    BWSolver();
    ~BWSolver();

    // clear solver states
    void Clear();

    void SetMapDimensions(int x, int y);

    void GetMapDimensions(int &x, int &y);

    void SetTile(int layer, int x,int y, Tile::Enum);

    Tile::Enum GetTile(int layer, int x,int y);

    void SetWorldMap(WorldMap & world);

    WorldMap * GetWorldMap();

    void SetSolutionMaxDepth(int depth);

    bool Solve();

    void AddObserver(BWSolverObserver * observer);

    WorldMap * GetSolutionWorldMap(int depth);

    Tile::Enum GetSolutionTile(int depth, int layer, int x, int y);

    int GetSolutionDepth();

    void StopSolving();

private:
    void HashFreeAll();
    StoreState * NewStoreState();
    bool SolveRecursive(State & currentstate);
    bool HashInsert(State & state);

    bool WasStopped;
    bool IsSolved;

    long StateTraversed;
    long StateHits;

    typedef std::set<StoreStateMemoryBlock*> StoreStateMemoryBlockSet;
    typedef std::set<StoreState*> StoreStateSet;
    typedef std::map<long, StoreStateSet> StoreStateMap;
    typedef std::pair<long, StoreStateSet> StoreStatePair;

    StoreStateMap mStoreStateMap;

    StoreStateMemoryBlockSet mStoreStateMemoryBlockList;
    StoreStateMemoryBlock *  mLastStoreStateMemoryBlock;

    // Only one observer for now
    BWSolverObserver * mObserver;

    int Depth; // current solution depth
    int mSolutionDepth;  
    State mStateArray[100];
    WorldMap mSolutionWorldMap[100];

    State mState;
    WorldMap mWorldMap;
    int mMaxDepth;
};



#endif