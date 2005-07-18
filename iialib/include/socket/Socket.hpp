#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <stdint.h>
//#include <vector>
#include <string>

#ifdef WINDOWS
#include <winsock.h>
#include <ws2tcpip.h>
#endif

#ifdef UNIX
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#endif

#include "exceptions/Exception.hpp"
#include "socket/InAddr.hpp"
#include "socket/InAddr4.hpp"
#include "socket/InAddr6.hpp"
#include "socket/HostEnt.hpp"
#include "socket/SockAddr.hpp"
#include "socket/SockAddrIn.hpp"
#include "socket/SockAddrIn6.hpp"



namespace IIALib
{
    namespace Socket
    {
        
        class Socket
        {
            public:
                
                enum {
                    ST_SOCK = 0,
                    ST_SOCK4,
                    ST_SOCK6
                };    
                
                enum {
                    FL_EXISTS    = 0x1,
                    FL_CONNECTED = 0x2
                };    
                
                #ifdef UNIX
                int    sSocket;
                #elsif WINDOWS
                SOCKET sSocket;
                #endif
                
                
                uint32_t iFlags;
                
                Socket();
                Socket(int iNameSpace, int iStyle, int iProtocol);
                ~Socket();
                
                void     Create(int iNameSpace = AF_INET, int iStyle = SOCK_STREAM, int iProtocol = IPPROTO_TCP);
                void     ShutDown(int iHow = 2);
                void     Close();
                
                void     Connect(SockAddr *pSockAddr);
                void     Listen(uint32_t iQueueLen);
                void     Bind(SockAddr *pSockAddr);
                Socket   Accept();
                Socket   Accept(SockAddr *&saSock);
                
                uint32_t Send(void *data, uint32_t iLen, uint32_t iInFlags = 0);
                uint32_t Recv(void *data, uint32_t iLen, uint32_t iInFlags = 0);
                
        };    
        
    };
    
};    

#endif
