#include "StoreStateMemoryBlock.h"
#include "StateClasses.h"


StoreStateMemoryBlock::StoreStateMemoryBlock()
{
    mMemoryBlock = new char[STORE_STATE_MEMORY_BLOCK_SIZE];
    mCurrentPosition = 0;
}
StoreStateMemoryBlock::~StoreStateMemoryBlock()
{
    delete mMemoryBlock;
}

StoreState * StoreStateMemoryBlock::Add(size_t storeStateSize)
{
    if (mCurrentPosition + storeStateSize > STORE_STATE_MEMORY_BLOCK_SIZE)
    {
        return NULL;
    } else
    {
        mCurrentPosition += storeStateSize;
        return (StoreState *) (mMemoryBlock + mCurrentPosition - storeStateSize);
    }
}

