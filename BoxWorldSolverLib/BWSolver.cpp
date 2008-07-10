#include "BWSolver.h"
#include "StateClasses.h"
#include "WorldMap.h"
#include "StoreStateMemoryBlock.h"

BWSolver::BWSolver() : mObserver(NULL)
{
    mLastStoreStateMemoryBlock = new StoreStateMemoryBlock();
    mStoreStateMemoryBlockList.insert(mLastStoreStateMemoryBlock);
}

BWSolver::~BWSolver()
{
    HashFreeAll();
}


void BWSolver::HashFreeAll()
{
    for (StoreStateMap::iterator iter = mStoreStateMap.begin();
        iter != mStoreStateMap.end();
        ++iter
        )
    {
        //Memory will be free'd in StoreStateMemoryBlocks!
        iter->second.clear();
    }
    mStoreStateMap.clear();

    for (StoreStateMemoryBlockSet::iterator iter2 = mStoreStateMemoryBlockList.begin();
        iter2 != mStoreStateMemoryBlockList.end();
        ++iter2)
    {
        delete *iter2;
    }
    mStoreStateMemoryBlockList.clear();

    mLastStoreStateMemoryBlock = new StoreStateMemoryBlock();
    mStoreStateMemoryBlockList.insert(mLastStoreStateMemoryBlock);
}

void BWSolver::SetMapDimensions(int x, int y)
{
    mWorldMap.SetXY(x, y);
    mWorldMap.Clear();
    mWorldMap.DrawBoundaries();
}

void BWSolver::GetMapDimensions(int &x, int &y)
{
    x = mWorldMap.GetX();
    y = mWorldMap.GetY();
}

void BWSolver::SetTile(int layer, int x,int y, Tile::Enum t)
{
    if (x<0 || x>=mWorldMap.GetX() ||
        y<0 || y>=mWorldMap.GetY())
    {
        throw BWSolverException("SetTile() coordinates outside map area!");
    }

    mWorldMap.SetTile(layer, x, y, t);
}

Tile::Enum BWSolver::GetTile(int layer, int x,int y)
{
    if (x<0 || x>=mWorldMap.GetX() ||
        y<0 || y>=mWorldMap.GetY())
    {
        throw BWSolverException("SetTile() coordinates outside map area!");
    }

    return mWorldMap.GetTile(layer, x, y);
}


Tile::Enum BWSolver::GetSolutionTile(int depth, int layer, int x, int y)
{
    return Tile::Empty;
}

int BWSolver::GetSolutionDepth()
{
    return mSolutionDepth;
}



void BWSolver::SetSolutionMaxDepth(int depth)
{
    mMaxDepth = depth;
}

void BWSolver::Clear()
{
    WasStopped = false;
    mSolutionDepth = 0;
    HashFreeAll();
    IsSolved = false;
    StateHits = 0;
    StateTraversed = 0;
    Depth = 0;
}


bool BWSolver::Solve()
{
    Clear();
    mWorldMap.Verify();
    mState.Copy(mWorldMap); // copy world map and set boxes/ player xy
    int x,y;
    mState.GetPlayerXY(x, y);
    mState.GetWorldMap().SetTile(1, x, y, Tile::Empty);
    mState.SetBoxFreedom();
    mState.depth = 0;
    HashInsert(mState);
    bool result = SolveRecursive(mState);


    if (WasStopped) throw BWSolverException("Solver stopped.");

    if (mObserver!=NULL)
    {
        mObserver->Progress(0, StateTraversed, mStoreStateMap.size(), StateHits);
        mObserver->Finished(mSolutionDepth);
    }

    return result;
}


bool BWSolver::SolveRecursive(State & currentstate)
{
    Depth++;

    IsSolved = currentstate.CheckWin();
    if (!IsSolved)
    {
        if (Depth<mMaxDepth)
            while (currentstate.CreateNextState(mStateArray[Depth]) && (!IsSolved))
            {
                mStateArray[Depth].depth = Depth;

                StateTraversed++;
                if (mStateArray[Depth].LegalState(mMaxDepth-Depth))
                {
                    mStateArray[Depth].SetBoxFreedom();

                    if (HashInsert(mStateArray[Depth]))
                    {
                        SolveRecursive(mStateArray[Depth]);
                    } 
                    else 
                        StateHits++;
                }
            }

            if ((mObserver!=NULL)&&(StateTraversed%1000==0))
            {
                mObserver->Progress(Depth, StateTraversed, mStoreStateMap.size(), StateHits);
            }
    }

    if (IsSolved)
    {
        if (Depth>mSolutionDepth)
        {
            mSolutionDepth = Depth;
        }

        int x,y;
        currentstate.GetPlayerXY(x, y);

        mSolutionWorldMap[Depth].Copy(currentstate.GetWorldMap());
        mSolutionWorldMap[Depth].SetTile(1, x, y, Tile::Player);
    }

    Depth--;
    return IsSolved;
}



StoreState * BWSolver::NewStoreState()
{
    // the memory for the store states are managed manually by StoreStateMemoryBlock
    StoreState * storeState = mLastStoreStateMemoryBlock->Add(sizeof(StoreState));
    if (storeState==NULL) // there is no more space in this memory block
    {
        mLastStoreStateMemoryBlock = new StoreStateMemoryBlock();
        mStoreStateMemoryBlockList.insert(mLastStoreStateMemoryBlock);
        storeState = mLastStoreStateMemoryBlock->Add(sizeof(StoreState)); // should always succeed
        if (storeState==NULL)
        {
            throw BWSolverException("HashInsert() This should not happen! Insufficient space for state in new state memory block!");
        }
    }
    return storeState;
}


bool BWSolver::HashInsert(State & state)
{
    long hash = state.HashValue();
    StoreStateMap::iterator iter = mStoreStateMap.find(hash);
    if (iter == mStoreStateMap.end())
    {
        StoreStateSet SSet;


        // the memory for the store states are managed manually by StoreStateMemoryBlock
        // freeing smaller quantity of large memory blocks is much faster
        StoreState * storeState = NewStoreState();

        storeState->Copy(state, state.GetNumberOfBoxes());
        SSet.insert(storeState);
        mStoreStateMap.insert(StoreStatePair(hash, SSet));
    }
    else
    {
        for (StoreStateSet::iterator iter2 = iter->second.begin();
            iter2 != iter->second.end();
            ++iter2
            )
        {
            if (state.Equals(**iter2))
            {
                return false;
                if (((**iter2)).depth <= state.depth)
                {
                    return false;
                }
                else
                {
                    ((**iter2)).depth = state.depth;
                    return true;
                }
            }
        }

        
        // the memory for the store states are managed manually by StoreStateMemoryBlock
        // freeing smaller quantity of large memory blocks is much faster
        StoreState * storeState = NewStoreState();

        storeState->Copy(state, state.GetNumberOfBoxes());
        iter->second.insert(storeState);
    }
    return true;
}


WorldMap * BWSolver::GetSolutionWorldMap(int depth)
{
    return &mSolutionWorldMap[depth];
}

void BWSolver::SetWorldMap(WorldMap & world)
{
    mWorldMap.Copy(world);
}

WorldMap * BWSolver::GetWorldMap()
{
    return &mWorldMap;
}

void BWSolver::AddObserver(BWSolverObserver * observer)
{
    mObserver = observer;
}

void BWSolver::StopSolving()
{
    IsSolved = true;
    WasStopped = true;
}