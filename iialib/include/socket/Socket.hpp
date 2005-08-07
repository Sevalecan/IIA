#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <setup.h>
#include <stdint.h>
//#include <vector>
#include <string>
#include <errno.h>

#ifdef WINDOWS
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

#ifdef UNIX
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
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
        
        class IIAEXPORT Socket
        {
            public:
                #ifdef WINDOWS
                static WSADATA wsaData;
                static WORD    wsaWord;
                #endif
                static void Init();
                static void Fini();
                
            public:
                
                enum SOCKTYPE {
                    ST_SOCK = 0,
                    ST_SOCK4,
                    ST_SOCK6
                };    
                
                enum FLAGS {
                    FL_EXISTS    = 0x1, // socket exists
                    FL_CONNECTED = 0x2  // socket is connected
                };    
                
                enum RETURN {
                    RET_INPROGRESS = 1,
                    RET_WOULDBLOCK
                };    
                
                #ifdef UNIX          // if on a unix-like system, we declare the socket descriptor as an int
                int    sSocket;
                #define IIA_SOCKTYPE int
                #elif WINDOWS        // otherwise, if its some piece of crap like windows which couldn't
                SOCKET sSocket;      // just be nice and comply with unix stuff then we declare it as a SOCKET
                #define IIA_SOCKTYPE SOCKET
                #endif               // because after all, this is C++ and it doesn't like implicit casts.
                
                uint32_t  iFlags;    // some state flags, see enum FLAGS. 
                sockaddr *pRemoteSA; // Remote sockaddr information allocated as new uint8_t[30].
                uint32_t  iSASize;   // size of data contained therein
                
                
            public:
                
                Socket();            // default constructor, does not create a socket.
                Socket(int iNameSpace, int iStyle, int iProtocol); // Creates a socket with the specified info.
                ~Socket();           // Destrcutor
                
                int32_t  Create(int iNameSpace = AF_INET, int iStyle = SOCK_STREAM, int iProtocol = IPPROTO_TCP);
                void     ShutDown(int iHow = 2);
                void     Close();
                
                int32_t  Connect(SockAddrIn &rSockAddr);
                int32_t  Connect(SockAddr *pSockAddr);
                int32_t  Listen(uint32_t iQueueLen);
                int32_t  Bind(SockAddr *pSockAddr);
                int32_t  Accept(Socket &iInSock);
                
                int32_t  Send(void *pData, uint32_t iLen, uint32_t iInFlags = 0);
                int32_t  Recv(void *pData, uint32_t iLen, uint32_t iInFlags = 0);
                
                int32_t  SendT(void *pData, uint32_t iLen, uint32_t iInFlags = 0);
                int32_t  RecvT(void *pData, uint32_t iLen, uint32_t iInFlags = 0);
                
                int32_t  IOCtl(uint32_t iCommand, void *pData);
                int32_t  GetSockOpt(int32_t iLevel, int32_t iOptName, void *pOptVal, socklen_t *slOptVal);
                int32_t  SetSockOpt(int32_t iLevel, int32_t iOptName, void *pOptVal, socklen_t slOptVal);
                // of course, for the above function(ioctl), microsoft has to take a completley
                // different approach to ioctl... >:E
                // THATS FOR MY WIFE, FUCK YOU AND DIE. *punches microshit* 
                
        };    
        
    };
    
};    

#endif
