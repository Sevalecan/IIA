#ifndef INADDR_HPP
#define INADDR_HPP

#include <stdint.h>

namespace IIALib
{
    namespace Socket
    {
        
        class InAddr
        {
            protected:
                InAddr() {};
            public:
                uint16_t iType;
                uint8_t  iData[0];
        };    
    };    
};    

#endif
