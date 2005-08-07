#define IIASOURCE
#include "socket/SockAddrIn.hpp"
#include "exceptions/Exception.hpp"

using namespace IIALib::Exceptions;

namespace IIALib
{
    
    namespace Socket
    {
        
        void SockAddrIn::FromStd(sockaddr_in *pInSock)
        {
            sockaddr_in *lpSock = pInSock;
            
            sinAddr.sAddr = lpSock->sin_addr.s_addr;
            saFamily = lpSock->sin_family;
            sinPort = lpSock->sin_port;
        }    
        
        SockAddrIn::SockAddrIn(sockaddr *pInSock)
        {
            FromStd((sockaddr_in *)pInSock);
        }
            
        SockAddrIn::SockAddrIn(sockaddr_in *pInSock)
        {
            FromStd(pInSock);
        }    
        
        SockAddrIn::SockAddrIn(sockaddr &rInSock)
        {
            FromStd((sockaddr_in *)(&rInSock));
        }    
        
        SockAddrIn::SockAddrIn(sockaddr_in &rInSock)
        {
            FromStd(&rInSock);
        }    
        
        SockAddrIn::saPType SockAddrIn::GetStdSockAddr()
        {
            uint32_t            iRet;
            struct sockaddr_in *pWork;
            saPType             saWork;
            
            iRet = saWork.Resize(sizeof(sockaddr_in));
            
            pWork = (struct sockaddr_in *)saWork.pData;
            
            pWork->sin_family        = saFamily;
            pWork->sin_addr.s_addr   = sinAddr.sAddr;
            pWork->sin_port          = sinPort;
            
            return saWork;
        }    
        
    }    
    
}

    
