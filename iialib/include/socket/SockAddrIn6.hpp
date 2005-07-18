#ifndef SOCKADDRIN6_HPP
#define SOCKADDRIN6_HPP

#include <stdint.h>

#ifdef WINDOWS
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

#ifdef UNIX
#include <sys/socket.h>
#include <netinet/in.h>
#endif

#include "socket/SockAddr.hpp"
#include "socket/InAddr6.hpp"

namespace IIALib
{
    namespace Socket
    {
        
        class SockAddrIn6 : public SockAddr
        {
            public:
                //sa_family_t sin6Family;
                InAddr6 sin6Addr;
                uint32_t sin6FlowInfo;
                uint16_t sin6Port;
                
                virtual saPType GetStdSockAddr();
                
        };    
        
    };    
};    

#endif
