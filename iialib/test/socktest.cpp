#include "socket/Socket.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#ifdef UNIX
#include <netdb.h>
#include <arpa/inet.h>
#endif
#ifdef WINDOWS
#include <windows.h>
#endif


using namespace std;
using namespace IIALib::Socket;
using namespace IIALib::Exceptions;

#define DEBUG printf("In File \"%s\" Current line: %d\n", __FILE__, __LINE__);

class TestRecv
{
    public:
        
        class TSocket : public Socket
        {
        	public:
        	int32_t RecvX(void *pData, uint32_t iLen, uint32_t iInFlags = 0);
        };
        
        TSocket sServer;
        uint32_t Recv();
};    

        int32_t TestRecv::TSocket::RecvX(void *pData, uint32_t iLen, uint32_t iInFlags)
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
        	    printf("                 \r");
        	    printf("%u/%u\r", ilProgress, iLen);
        	}
        	printf("\n");
            return ilProgress;
        }    
        

class TestServ
{
    public:
        
        Socket sListen;
        sockaddr_in wtf;
        SockAddrIn saLoc;
        
        void      Create(int32_t iPort);
        void      Accept(TestRecv &trIn);
        
};    

void TestServ::Create(int32_t iPort)
{
    sListen.Create(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    wtf.sin_family = AF_INET;
    wtf.sin_addr.s_addr = INADDR_ANY;
    wtf.sin_port  = htons(iPort);
    
    saLoc = wtf;
    
    sListen.Bind(&saLoc);
    sListen.Listen(5);
}    

void TestServ::Accept(TestRecv &trIn)
{
    sListen.Accept(trIn.sServer);
}    

uint32_t TestRecv::Recv()
{
	fstream out;
	uint32_t iStrLen;
	uint32_t iDatLen;
    string filename;
    vector<uint8_t> data;
    
    sServer.RecvT((void *)&iDatLen, 4);
    sServer.RecvT((void *)&iStrLen, 4);
    filename.resize(iStrLen);
    data.resize(iDatLen);
    sServer.RecvT((void *)filename.c_str(), iStrLen);
    sServer.RecvX((void *)&data[0], iDatLen);
    
    out.open("test.png", fstream::binary | fstream::out);
    
    out.write((char *)(&data[0]), iDatLen);
    data.resize(0);
    
    printf("Length: %u\nName: %s\nNameLen: %u\n", iDatLen, filename.c_str(), iStrLen);
    
    out.close();
    return 0;
}

int main(int argc, char *argv[])
{
    TestServ tsA;
    TestRecv tsR;
    struct linger lFuck;
    int v = 0;
    int x;
    char dat[11];
    
    lFuck.l_onoff = 0;
    lFuck.l_linger = 1;
    
    memset(dat, 0, 11);
    
    Socket::Init();
    
    tsA.Create(50500);
    tsA.Accept(tsR);
    tsA.sListen.Close();
    
    tsR.Recv();
    cout << errno << endl;
    tsR.sServer.Close();
    
    Socket::Fini();
    return 0;
}

    
