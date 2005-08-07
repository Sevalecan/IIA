#define IIASOURCE
#include "socket/Socket.hpp"


using namespace IIALib::Exceptions;

namespace IIALib
{
    
    namespace Socket
    {
        #ifdef WINDOWS
        WSADATA Socket::wsaData;
        WORD    Socket::wsaWord;
        #endif
        
        void Socket::Init()
        {
            #ifdef WINDOWS
            wsaWord = MAKEWORD(1,1);   
            WSAStartup(wsaWord, &wsaData);
            #endif
        }
            
        void Socket::Fini()
        {
            #ifdef WINDOWS
            WSACleanup();
            #endif
        }    
        
        Socket::Socket()
        {
            iFlags = 0;
            pRemoteSA = (sockaddr *)new uint8_t[30];
            iSASize = 0;
        }    
        
        Socket::Socket(int iNameSpace, int iStyle, int iProtocol)
        {
            int32_t iRet;
            
            iFlags = 0;
            pRemoteSA = (sockaddr *)new uint8_t[30];
            iSASize = 0;
            
            iRet = Create(iNameSpace, iStyle, iProtocol);
        }    
        
        Socket::~Socket()
        {
            Close();
            delete [] (uint8_t *)pRemoteSA;
        }    
        
        int32_t Socket::Create(int iNameSpace, int iStyle, int iProtocol)
        {
            if ((iFlags & FL_EXISTS))
                Close();
            
            sSocket = ::socket(iNameSpace, iStyle, iProtocol);
            
            if ((int32_t)sSocket == -1)
                return -1;
            
            iFlags |= FL_EXISTS;
        }    
        
        void Socket::Close()
        {
            if (sSocket)
            {
                #ifdef UNIX
                ::close(sSocket);
                #elif WINDOWS
                ::closesocket(sSocket);
                #endif
            }    
            
            iFlags &= ~FL_EXISTS;
            sSocket = 0;
        }    
        
        void Socket::ShutDown(int iHow)
        {
            if (sSocket)
                ::shutdown(sSocket, iHow);
        }    
        
        int32_t Socket::Connect(SockAddrIn &rSockAddr)
        {  
            return Connect((SockAddr *)&rSockAddr);
        }    
        
        int32_t Socket::Connect(SockAddr *pSockAddr)
        {
            int32_t          iRet;
            SockAddr::saPType saSock;
            
            saSock = pSockAddr->GetStdSockAddr();
                    
            iRet = ::connect(sSocket, (sockaddr *)saSock.pData, saSock.iLength);
            
            memcpy(pRemoteSA, (sockaddr *)saSock.pData, saSock.iLength);
            iSASize = saSock.iLength;
            
            
            return iRet;
            
        }    
        
        int32_t Socket::Listen(uint32_t iQueueLen)
        {
            int32_t iRet;
            
            iRet = ::listen(sSocket, iQueueLen);
            return iRet;
        }    
        
        int32_t Socket::Bind(SockAddr *pSockAddr)
        {
            SockAddr::saPType  saSock;
            uint32_t           ilRet;
            
            saSock = pSockAddr->GetStdSockAddr();
            
            ilRet = ::bind(sSocket, (sockaddr *)saSock.pData, saSock.iLength);
            return ilRet;
        }    
        
        int32_t Socket::Accept(Socket &iInSock)
        {
            IIA_SOCKTYPE slSock;
            uint8_t      ilData[30];
            sockaddr    *plSockAddr = (sockaddr *)ilData;
            socklen_t    slSize = 30;
            int32_t      iRet;
            
            
            slSock = ::accept(sSocket, plSockAddr, &slSize);
            
            if (slSock == -1)
                return -1; 
            
            iInSock.Close();
            iInSock.sSocket = slSock;
            memcpy(iInSock.pRemoteSA, plSockAddr, slSize);
            iInSock.iSASize = slSize;
            iInSock.iFlags = FL_EXISTS | FL_CONNECTED;
            
            return 0;
            
        }    
        
        int32_t Socket::Send(void *pData, uint32_t iLen, uint32_t iInFlags)
        {   
            return ::send(sSocket, (const char *)pData, iLen, iInFlags);
        }    
        
        int32_t Socket::Recv(void *pData, uint32_t iLen, uint32_t iInFlags)
        {
            
            return ::recv(sSocket, (char *)pData, iLen, iInFlags);
        }    
        
        
        int32_t Socket::SendT(void *pData, uint32_t iLen, uint32_t iInFlags)
        {   
        	int32_t  ilRet;
        	int32_t  ilRetf;
        	uint32_t ilProgress;
        	
        	ilProgress = 0;
        	
        	while (ilProgress < iLen)
        	{
        	    ilRetf = ::send(sSocket, (char *)pData+ilProgress, iLen-ilProgress, iInFlags);
        	    if (ilRetf > 0)
        	        ilProgress += ilRetf;
        	    else
        	    {
        	    	#ifdef UNIX
        	    	if (errno != EWOULDBLOCK && errno != ENOBUFS && errno != EINTR)
        	    	    return ilProgress;
        	    	#elif WINDOWS
        	    	if (WSAGetLastError() != WSAEINPROGRESS && WSAGetLastError() != WSAEWOULDBLOCK && WSAGetLastError() != WSAEMSGSIZE && WSAGetLastError() != WSAEINTR)
        	    	    return ilProgress;
        	    	#endif
        	    }
        	}
        	
            return ilProgress;
        }    
        
        int32_t Socket::RecvT(void *pData, uint32_t iLen, uint32_t iInFlags)
        {
            int32_t  ilRet;
        	int32_t  ilRetf;
        	uint32_t ilProgress;
        	
        	ilProgress = 0;
        	
        	while (ilProgress < iLen)
        	{
        	    ilRetf = ::recv(sSocket, (char *)pData+ilProgress, iLen-ilProgress, iInFlags);
        	    if (ilRetf > 0)
        	        ilProgress += ilRetf;
        	    else if (ilRetf < 0)
        	    {
        	    	#ifdef UNIX
        	    	if (errno != EWOULDBLOCK && errno != EINTR)
        	    	    return ilProgress;
        	    	#elif WINDOWS
        	    	if (WSAGetLastError() != WSAEINPROGRESS && WSAGetLastError() != WSAEWOULDBLOCK && WSAGetLastError() != WSAEMSGSIZE && WSAGetLastError() != WSAEINTR)
        	    	    return ilProgress;
        	    	#endif
        	    }
        	}
        	
            return ilProgress;
        }    
        
        
        int32_t Socket::IOCtl(uint32_t iCommand, void *pData)
        {
            
            #ifdef UNIX
            return ioctl(sSocket, iCommand, pData);
            #elif WINDOWS
            return ioctlsocket(sSocket, iCommand, (u_long *)pData);
            #endif
            
        }
        
        int32_t Socket::GetSockOpt(int32_t iLevel, int32_t iOptName, void *pOptVal, socklen_t *slOptVal)
        {
            return ::getsockopt(sSocket, iLevel, iOptName, (char *)pOptVal, slOptVal);
        }    
        
        int32_t Socket::SetSockOpt(int32_t iLevel, int32_t iOptName, void *pOptVal, socklen_t slOptVal)
        {
            return ::setsockopt(sSocket, iLevel, iOptName, (char *)pOptVal, slOptVal);
        }    
        
    }    
    
}
    
