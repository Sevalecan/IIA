#ifndef INADDR6_HPP
#define INADDR6_HPP

#include <stdint.h>

#ifdef WINDOWS
#include <winsock.h>
#endif

#ifdef UNIX
#include <sys/socket.h>
#endif

#include "socket/InAddr.hpp"

namespace IIALib
{
    namespace Socket
    {
        
        class InAddr6 : public InAddr
        {
            public:
                
                union {
                    uint8_t   u6_addr8[16];
                    uint16_t  u6_addr16[8];
                    uint32_t  u6_addr32[4];
                } uIn6;    
                
                InAddr6() { iType = AF_INET6; };
                
        };    
    };
};    

#endif
