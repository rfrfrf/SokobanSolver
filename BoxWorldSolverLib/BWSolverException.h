// BWSolverException.h: interface for the BWSolverException class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BWSOLVEREXCEPTION_H__D4337E61_3E5F_11D9_9525_0050BF096855__INCLUDED_)
#define AFX_BWSOLVEREXCEPTION_H__D4337E61_3E5F_11D9_9525_0050BF096855__INCLUDED_

#include <string>

class BWSolverException  
{
public:
    BWSolverException(std::string error);
    virtual ~BWSolverException();

    std::string GetErrorMessage();
private:
    std::string mMessage;
};

#endif // !defined(AFX_BWSOLVEREXCEPTION_H__D4337E61_3E5F_11D9_9525_0050BF096855__INCLUDED_)
