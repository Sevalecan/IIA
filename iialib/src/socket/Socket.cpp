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
        
        Socket::Instance::Instance()
        {
            
            iFlags = 0;
            pRemoteSA = (sockaddr *)new uint8_t[30];
            iSASize = 0;
            iRefCount = 1;
            
            if (!pRemoteSA)
                throw Exception("Could not allocate data for socket instance.", E_NOMEMORY);
            
        }
        
        Socket::Instance::~Instance()
        {
            
            delete [] (uint8_t *)pRemoteSA;
            
        }
        
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
        	cInst = NULL;
            NewInstance();
            if (!cInst)
                throw Exception("Could not allocate data for socket instance.", E_NOMEMORY);
            
        }    
        
        Socket::Socket(int iNameSpace, int iStyle, int iProtocol)
        {
            int32_t iRet;
            
            cInst = NULL;
            
            NewInstance();
            if (!cInst)
                throw Exception("Could not allocate data for socket instance.", E_NOMEMORY);
            
            iRet = Create(iNameSpace, iStyle, iProtocol);
        }    
        
        Socket::~Socket()
        {
        	if (cInst->iRefCount == 1)
        	{
                Close();
                delete cInst;
        	}
        }    
        
        int32_t Socket::NewInstance()
        {
            
            if (cInst)
            {
                if (cInst->iRefCount <= 1)
                {
                    cInst->iRefCount--;
                    cInst = NULL;
                    cInst = new Instance();
                }
            }
            else if (!cInst)
                cInst = new Instance();
            
            return 0;
        }
        
        int32_t Socket::CutInstance()
        {
            
            if (cInst && cInst->iRefCount > 1)
            {
                cInst->iRefCount--;
                cInst = NULL;
            }
            else if (cInst && cInst->iRefCount == 1)
            {
                delete cInst;
                cInst = NULL;
            }
                
            
            return 0;
        }
        
        int32_t Socket::Create(int iNameSpace, int iStyle, int iProtocol)
        {
        	cInst->cMutex.Lock();
            if ((cInst->iFlags & FL_EXISTS))
                Close();
            
            cInst->sSocket = ::socket(iNameSpace, iStyle, iProtocol);
            
            if ((int32_t)cInst->sSocket == -1)
                return -1;
            
            cInst->iFlags |= FL_EXISTS;
            cInst->cMutex.UnLock();
            
            return 0;
        }    
        
        void Socket::Close()
        {
        	cInst->cMutex.Lock();
            if (cInst->sSocket)
            {
                #ifdef UNIX
                ::close(cInst->sSocket);
                #elif WINDOWS
                ::closesocket(cInst->sSocket);
                #endif
            }    
            
            cInst->iFlags &= ~FL_EXISTS;
            cInst->sSocket = 0;
            cInst->cMutex.UnLock();
        }    
        
        void Socket::ShutDown(int iHow)
        {
        	cInst->cMutex.Lock();
            if (cInst->sSocket)
                ::shutdown(cInst->sSocket, iHow);
            cInst->cMutex.UnLock();
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
            
            cInst->cMutex.Lock();
            iRet = ::connect(cInst->sSocket, (sockaddr *)saSock.pData, saSock.iLength);
            
            memcpy(cInst->pRemoteSA, (sockaddr *)saSock.pData, saSock.iLength);
            cInst->iSASize = saSock.iLength;
            cInst->cMutex.UnLock();
            
            return iRet;
            
        }    
        
        int32_t Socket::Listen(uint32_t iQueueLen)
        {
            int32_t iRet;
            
            cInst->cMutex.Lock();
            iRet = ::listen(cInst->sSocket, iQueueLen);
            cInst->cMutex.UnLock();
            
            return iRet;
        }    
        
        int32_t Socket::Bind(SockAddr *pSockAddr)
        {
            SockAddr::saPType  saSock;
            uint32_t           ilRet;
            
            saSock = pSockAddr->GetStdSockAddr();
            cInst->cMutex.Lock();
            ilRet = ::bind(cInst->sSocket, (sockaddr *)saSock.pData, saSock.iLength);
            cInst->cMutex.UnLock();
            
            return ilRet;
        }    
        
        int32_t Socket::Accept(Socket &iInSock)
        {
            IIA_SOCKTYPE slSock;
            uint8_t      ilData[30];
            sockaddr    *plSockAddr = (sockaddr *)ilData;
            socklen_t    slSize = 30;
            int32_t      iRet;
            
            cInst->cMutex.Lock();
            
            slSock = ::accept(cInst->sSocket, plSockAddr, &slSize);
            
            if (slSock == -1) {
                cInst->cMutex.UnLock();
                return -1; 
            }
            
            iInSock.Close();
            iInSock.cInst->sSocket = slSock;
            memcpy(iInSock.cInst->pRemoteSA, plSockAddr, slSize);
            iInSock.cInst->iSASize = slSize;
            iInSock.cInst->iFlags = FL_EXISTS | FL_CONNECTED;
            
            cInst->cMutex.UnLock();
            
            return 0;
            
        }    
        
        int32_t Socket::Send(void *pData, uint32_t iLen, uint32_t iInFlags)
        {   
        	int32_t ilRet;
        	
        	cInst->cMutex.Lock();
            ilRet = ::send(cInst->sSocket, (const char *)pData, iLen, iInFlags);
            cInst->cMutex.UnLock();
            
            return ilRet;
        }    
        
        int32_t Socket::Recv(void *pData, uint32_t iLen, uint32_t iInFlags)
        {
            int32_t ilRet;
            
            cInst->cMutex.Lock();
            ilRet = ::recv(cInst->sSocket, (char *)pData, iLen, iInFlags);
            cInst->cMutex.UnLock();
            
            return ilRet;
        }    
        
        
        int32_t Socket::SendT(void *pData, uint32_t iLen, uint32_t iInFlags)
        {   
        	int32_t  ilRet;
        	int32_t  ilRetf;
        	uint32_t ilProgress;
        	
        	ilProgress = 0;
        	
        	cInst->cMutex.Lock();
        	
        	while (ilProgress < iLen)
        	{
        	    ilRetf = ::send(cInst->sSocket, (char *)pData+ilProgress, iLen-ilProgress, iInFlags);
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
        	
        	cInst->cMutex.UnLock();
        	
            return ilProgress;
        }    
        
        int32_t Socket::RecvT(void *pData, uint32_t iLen, uint32_t iInFlags)
        {
            int32_t  ilRet;
        	int32_t  ilRetf;
        	uint32_t ilProgress;
        	
        	ilProgress = 0;
        	
        	cInst->cMutex.Lock();
        	
        	while (ilProgress < iLen)
        	{
        	    ilRetf = ::recv(cInst->sSocket, (char *)pData+ilProgress, iLen-ilProgress, iInFlags);
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
        	
        	cInst->cMutex.UnLock();
        	
            return ilProgress;
        }    
        
        
        int32_t Socket::SendTo(void *pData, uint32_t iLen, uint32_t iInFlags, SockAddrIn &saIn)
        {   
        	int32_t ilRet;
        	SockAddr::saPType spSocket;
        	spSocket = saIn.GetStdSockAddr();
        	
        	cInst->cMutex.Lock();
            ilRet = ::sendto(cInst->sSocket, (const char *)pData, iLen, iInFlags, (sockaddr *)spSocket.pData, spSocket.iLength);
            cInst->cMutex.UnLock();
            
            return ilRet;
        }    
        
        int32_t Socket::RecvFrom(void *pData, uint32_t iLen, uint32_t iInFlags, SockAddrIn &saOut)
        {
            int32_t ilRet;
            SockAddr::saPType spSocket;
            spSocket.Resize(32);
            
            cInst->cMutex.Lock();
            #ifdef UNIX
            ilRet = ::recvfrom(cInst->sSocket, (char *)pData, iLen, iInFlags, (sockaddr *)spSocket.pData, (socklen_t *)&(spSocket.iLength));
            #elif WINDOWS
            ilRet = ::recvfrom(cInst->sSocket, (char *)pData, iLen, iInFlags, (sockaddr *)spSocket.pData, (int *)&(spSocket.iLength));
            #endif
            cInst->cMutex.UnLock();
            
            saOut = (sockaddr_in *)spSocket.pData;
            
            return ilRet;
        }    
        
        int32_t Socket::IOCtl(int32_t iCommand, void *pData)
        {
        	int32_t ilRet;
            
            cInst->cMutex.Lock();
            #ifdef UNIX
            ilRet = fcntl(cInst->sSocket, iCommand, pData);
            #elif WINDOWS
            ilRet = ioctlsocket(cInst->sSocket, iCommand, (u_long *)pData);
            #endif
            cInst->cMutex.UnLock();
            
            return ilRet;
        }
        
        int32_t Socket::GetSockOpt(int32_t iLevel, int32_t iOptName, void *pOptVal, socklen_t *slOptVal)
        {
        	int32_t ilRet;
        	
        	cInst->cMutex.Lock();
            ilRet = ::getsockopt(cInst->sSocket, iLevel, iOptName, (char *)pOptVal, slOptVal);
            cInst->cMutex.UnLock();
            
            return ilRet;
        }    
        
        int32_t Socket::SetSockOpt(int32_t iLevel, int32_t iOptName, void *pOptVal, socklen_t slOptVal)
        {
        	int32_t ilRet;
        	
        	cInst->cMutex.Lock();
            ilRet = ::setsockopt(cInst->sSocket, iLevel, iOptName, (char *)pOptVal, slOptVal);
            cInst->cMutex.UnLock();
            
            return ilRet;
        }    
        
        int32_t Socket::Select(int16_t ilFlags, struct timeval *timeout)
        {
            int32_t ilRet;
            fd_set flRSet;
            fd_set flWSet;
            fd_set flESet;
            
            FD_ZERO(&flRSet);
            FD_ZERO(&flWSet);
            FD_ZERO(&flESet);
            FD_SET(cInst->sSocket, &flRSet);
            FD_SET(cInst->sSocket, &flWSet);
            FD_SET(cInst->sSocket, &flESet);
            
            cInst->cMutex.Lock();
            
            if (ilFlags) {
                ilRet = ::select(FD_SETSIZE, (ilFlags & 1) ? &flRSet : NULL, (ilFlags & 2) ? &flWSet : NULL, (ilFlags & 4) ? &flESet : NULL, timeout);
                ilRet = (FD_ISSET(cInst->sSocket, &flESet) ? 1:0 << 2) | (FD_ISSET(cInst->sSocket, &flWSet) ? 1:0 << 1) | (FD_ISSET(cInst->sSocket, &flRSet)?1:0);
            }
            else
                ilRet = 0;
            
            cInst->cMutex.UnLock();
            
            return ilRet;
        }
        
        Socket &Socket::operator =(Socket &ilSock)
        {
        	CutInstance();
        	cInst = ilSock.cInst;
        	return *this;
        }
        
    }    
    
}
    
