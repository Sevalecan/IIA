#define IIASOURCE
#include "socket/SockAddrIn6.hpp"


/*
you know, it would probably take less room if I didnt copy most of the code in these constructors...
Oh well, it'd be the same if I did a macro or inline, though I doubt the performace lost if I make a normal
function would be anything to worry about

Though an inline function would make sense were I to change any of that code, so that's what I'll do
*/


namespace IIALib
{
    
    namespace Socket
    {
        void SockAddrIn6::FromStd(sockaddr_in6 *pInSock)
        {
            sockaddr_in6 *lpSock = pInSock;
            in6_addr     *lpAddr = &(lpSock->sin6_addr);
            
            saFamily = lpSock->sin6_family;
            memcpy(sin6Addr.uIn6.u6_addr8, lpAddr, sizeof(struct in6_addr));
            sin6FlowInfo = lpSock->sin6_flowinfo;
            sin6Port = lpSock->sin6_port;
            
        }    
        
        SockAddrIn6::SockAddrIn6(sockaddr *pInSock)
        {
            FromStd((sockaddr_in6 *)pInSock);
        }    
        
        SockAddrIn6::SockAddrIn6(sockaddr_in6 *pInSock)
        {
            FromStd(pInSock);
        }    
        
        SockAddrIn6::SockAddrIn6(sockaddr &rInSock)
        {
            FromStd((sockaddr_in6 *)&rInSock);
        }    
        
        SockAddrIn6::SockAddrIn6(sockaddr_in6 &rInSock)
        {
            FromStd(&rInSock);
        }    
        
        SockAddrIn6::saPType SockAddrIn6::GetStdSockAddr()
        {
            uint32_t             iRet;
            struct sockaddr_in6 *pWork;
            saPType              saWork;
            
            iRet = saWork.Resize(sizeof(struct sockaddr_in6));
            
            pWork = (struct sockaddr_in6 *)saWork.pData;
            
            pWork->sin6_family        = saFamily;
            #ifndef CYGWIN
            memcpy(pWork->sin6_addr.s6_addr32, sin6Addr.uIn6.u6_addr32, 4*sizeof(uint32_t));
            #else
            memcpy(pWork->sin6_addr.s6_addr, sin6Addr.uIn6.u6_addr8, 4*sizeof(uint32_t));
            #endif
            pWork->sin6_port          = sin6Port;
            
            return saWork;
        }
        
    }
        
}

    
