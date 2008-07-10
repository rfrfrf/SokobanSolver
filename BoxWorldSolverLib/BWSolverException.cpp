// BWSolverException.cpp: implementation of the BWSolverException class.
//
//////////////////////////////////////////////////////////////////////

#include "BWSolverException.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BWSolverException::BWSolverException(std::string m)
{
    mMessage = m;
}

BWSolverException::~BWSolverException()
{

}

std::string BWSolverException::GetErrorMessage()
{
    return mMessage;
}
