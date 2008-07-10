#ifndef __StoreStateMemoryBlock__
#define __StoreStateMemoryBlock__

#define STORE_STATE_MEMORY_BLOCK_SIZE 10485760

class StoreState;

class StoreStateMemoryBlock
{
public:
    StoreStateMemoryBlock();
    ~StoreStateMemoryBlock();

    StoreState * Add(size_t storeStateSize);

private:
    char * mMemoryBlock;
    size_t mCurrentPosition;
};






#endif __StoreStateMemoryBlock__