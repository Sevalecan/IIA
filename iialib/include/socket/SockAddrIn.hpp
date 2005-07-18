#ifndef SOCKADDRIN_HPP
#define SOCKADDRIN_HPP

#include <stdint.h>

#ifdef WINDOWS
#include <winsock.h>
#endif

#ifdef UNIX
#include <sys/socket.h>
#include <netinet/in.h>
#endif

#include "socket/SockAddr.hpp"
#include "socket/InAddr4.hpp"

namespace IIALib
{
    namespace Socket
    {
        
        class SockAddrIn : public SockAddr
        {
            public:
                InAddr4 sinAddr;
                uint16_t sinPort;
                
                virtual saPType GetStdSockAddr();
                
        };    
        
    };    
};    

#endif
