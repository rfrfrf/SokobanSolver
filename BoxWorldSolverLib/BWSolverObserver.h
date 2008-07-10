#ifndef __BW_SOLVER_OBSERVER__
#define __BW_SOLVER_OBSERVER__

#pragma warning(disable:4786)


class BWSolverObserver
{
public:
    virtual void Progress(long depth,
        long statesTraversed,
        long statesStored,
        long recurrentStoredStates
        )=0;
    virtual void Finished(int solutionDepth)=0;
};








#endif