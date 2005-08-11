#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <setup.h>
#include <string>
#include <stdint.h>

namespace IIALib
{
    
    namespace Exceptions
    {
        
        enum E_ERRORS
        {
            E_UNKNOWN = 0,
            E_NOMEMORY,
            E_INVALID
        };
                
        enum E_LEVELS
        {
            E_LOW = 0,
            E_MEDIUM,
            E_HIGH,
            E_CRITICAL
        };
        
        class IIAEXPORT Exception
        {
            protected:
                Exception() {};
                
            public:
                
                uint32_t     iLevel;
                int32_t      iError;
                std::string  sError;
                
                Exception(std::string sInError, int32_t iInError, uint32_t iInLevel);
                Exception(std::string sInError, int32_t iInError);
                
        };    
        
    }    
    
}    





#endif
