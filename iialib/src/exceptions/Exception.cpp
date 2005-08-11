#define IIASOURCE
#include "exceptions/Exception.hpp"

namespace IIALib
{
    
    namespace Exceptions
    {
        
        Exception::Exception(std::string sInError, int32_t iInError, uint32_t iInLevel)
        {
            iError = iInError;
            sError = sInError;
            iLevel = iInLevel;
        }    
        
        Exception::Exception(std::string sInError, int32_t iInError)
        {
            iError = iInError;
            sError = sInError;
            iLevel = 0;
        }    
        
    }    
    
}
    
