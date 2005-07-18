#include "exceptions/Exception.hpp"

namespace IIALib
{
    
    namespace Exceptions
    {
        
        Exception::Exception(std::string sInError, uint32_t iInError, uint32_t iInLevel)
        {
            iError = iInError;
            sError = sInError;
            iLevel = iInLevel;
        }    
        
    }    
    
}
    
