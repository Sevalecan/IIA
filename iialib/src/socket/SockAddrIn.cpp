#include "socket/SockAddrIn.hpp"
#include "exceptions/Exception.hpp"

using namespace IIALib::Exceptions;

namespace IIALib
{
    
    namespace Socket
    {
        
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

    
