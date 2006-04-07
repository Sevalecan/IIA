#ifndef SOCKADDRIN_HPP
#define SOCKADDRIN_HPP

#include <setup.h>
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
        
        class IIAEXPORT SockAddrIn : public SockAddr
        {
            public:
                InAddr4 sinAddr;
                uint16_t sinPort;
                
                SockAddrIn() { saFamily = AF_INET; };
                SockAddrIn(sockaddr *pInSock);
                SockAddrIn(sockaddr_in *pInSock);
                SockAddrIn(sockaddr &rInSock);
                SockAddrIn(sockaddr_in &rInSock);
                
                virtual saPType GetStdSockAddr();
                
            protected:
                inline void IIACALL FromStd(sockaddr_in *pInSock);
                
        };    
        
    };    
};    

#endif
