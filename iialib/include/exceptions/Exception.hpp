#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <string>
#include <stdint.h>

namespace IIALib
{
    
    namespace Exceptions
    {
        
        class Exception
        {
            protected:
                Exception() {};
                
            public:
                
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
                
                        
                
                uint32_t     iLevel;
                int32_t      iError;
                std::string  sError;
                
                Exception(std::string sInError, int32_t iInError, uint32_t iInLevel);
                
        };    
        
    }    
    
}    

{




#endif
