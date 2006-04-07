#ifndef SOCKADDRIN6_HPP
#define SOCKADDRIN6_HPP

#define USE_IPV6

#include <setup.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

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
        
        class IIAEXPORT SockAddrIn6 : public SockAddr
        {
            public:
                //sa_family_t sin6Family;
                InAddr6 sin6Addr;
                uint32_t sin6FlowInfo;
                uint16_t sin6Port;
                
                SockAddrIn6(sockaddr *pInSock);
                SockAddrIn6(sockaddr_in6 *pInSock);
                SockAddrIn6(sockaddr &rInSock);
                SockAddrIn6(sockaddr_in6 &rInSock);
                
                virtual saPType IIACALL GetStdSockAddr();
                
            protected:
                inline void FromStd(sockaddr_in6 *pInSock);
                
        };    
        
    };    
};    

#endif
