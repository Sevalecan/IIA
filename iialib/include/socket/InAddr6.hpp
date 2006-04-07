#ifndef INADDR6_HPP
#define INADDR6_HPP

#include <setup.h>
#include <stdint.h>

#define USE_IPV6

#ifdef WINDOWS
#include <winsock.h>
#endif

#ifdef UNIX
#include <sys/socket.h>
#endif

#include "socket/InAddr.hpp"

#ifndef AF_INET6
#define AF_INET6 10
#endif

namespace IIALib
{
    namespace Socket
    {
        
        class IIAEXPORT InAddr6 : public InAddr
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
