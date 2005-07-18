#include "socket/SockAddrIn6.hpp"

namespace IIALib
{
    
    namespace Socket
    {
        
        SockAddrIn6::saPType SockAddrIn6::GetStdSockAddr()
        {
            uint32_t             iRet;
            struct sockaddr_in6 *pWork;
            saPType              saWork;
            
            iRet = saWork.Resize(sizeof(struct sockaddr_in6));
            
            pWork = (struct sockaddr_in6 *)saWork.pData;
            
            pWork->sin6_family        = saFamily;
            memcpy(pWork->sin6_addr.s6_addr32, sin6Addr.uIn6.u6_addr32, 4*sizeof(uint32_t));
            pWork->sin6_port          = sin6Port;
            
            return saWork;
        }
        
    }
        
}

    
