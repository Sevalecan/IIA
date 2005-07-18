#include "socket/Socket.hpp"

namespace IIALib
{
    
    namespace Socket
    {
        
        Socket::Socket()
        {
            flags = 0;
        }    
        
        Socket::Socket(int iNameSpace, int iStyle, int iProtocol)
        {
            flags = 0;
            Create(iNameSpace, iStyle, iProtocol);
        }    
        
        Socket::~Socket()
        {
            Close();
        }    
        
        void Socket::Create(int iNameSpace, int iStyle, int iProtocol)
        {
            int32_t iRet;
            if (!(flags & FL_EXISTS))
                Close();
            
            sSocket = ::socket(iNameSpace, iStyle, iProtocol);
            if ((int32_t)sSocket <= 0) {
                iRet = sSocket;
                sSocket = 0;
                throw Exception("Invalid socket descriptor.", E_INVALID, E_MEDIUM);
            }    
            flags |= FL_EXISTS;
        }    
        
        void Socket::Close()
        {
            if (sSocket)
            {
                #ifdef UNIX
                ::close(sSocket);
                #elseif WINDOWS
                ::closesocket(sSocket);
                #endif
            }    
            
            flags &= ~FL_EXISTS;
            sSocket = 0;
        }    
        
        void Socket::ShutDown(int iHow)
        {
            if (sSocket)
                ::shutdown(sSocket, iHow);
        }    
        
        void Socket::Connect(SockAddr *pSockAddr)
        {
            int32_t          iRet;
            SockAddr::saPType saSock;
            
            if (!pSockAddr)
                throw Exception("Invalid SockAddr pointer.", E_INVALID, E_MEDIUM);
            
            saSock = pSockAddr->GetStdSockAddr();
            
            if (sSocket)
                iRet = ::connect(sSocket, saSock.pData, saSock.iLength);
            if (iRet <= 0)
                throw Exception("Unable to connect.", E_INVALID, E_MEDIUM);
            
        }    
        
        void Socket::Listen(uint32_t iQueueLen)
        {
            uint32_t iRet;
            if (!sSocket)
                throw Exception("Invalid socket descriptor.", E_INVALID, E_MEDIUM);
            
            iRet = ::listen(sSocket, iQueueLen);
            if (iRet)
                throw Exception("Unable to listen.", E_UNKNOWN, E_MEDIUM);
        }    
        
        void Socket::Bind(SockAddr *pSockAddr)
        {
            saPType  saSock;
            uint32_t iRet;
            
            if (!sSocket)
                throw Exception("Invalid socket descriptor.", E_INVALID, E_MEDIUM);
            
            if (!pSockAddr)
                throw Exception("Invalid SockAddr pointer.", E_INVALID, E_MEDIUM);
            
            saSock = pSockAddr->GetStdSockAddr();
            
            iRet = ::bind(sSocket, saSock.pData, saSock.iLength);
            if (iRet)
                throw Exception("Unable to bind to an address.", E_UNKNOWN, E_MEDIUM);
        }    
        
        Socket Accept(SockAddr *&saSock)
        {
            sockaddr *saFir;
            SockAddr *saSec;
            
            if (!sSocket)
                throw Exception("Invalid socket descriptor.", E_INVALID, E_MEDIUM);
            
            saSock = saSec;
            delete saFir;
        }    
        
        Socket Socket::Accept()
        {
            
        }    
        
    }    
    
}
    
