#include "socket/Socket.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <time.h>
#include <curses.h>
#ifdef UNIX
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/time.h>
#endif
#ifdef WINDOWS
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#endif


using namespace std;
using namespace IIALib::Socket;
using namespace IIALib::Exceptions;

#define DEBUG printf("In File \"%s\" Current line: %d\n", __FILE__, __LINE__);

#ifdef WINDOWS
int gettimeofday(struct timeval *tp, void *tzp)
{
	LARGE_INTEGER llA, llB;
	QueryPerformanceFrequency(&llA);
	QueryPerformanceCounter(&llB);
	
	tp->tv_sec  = (int)(llB.QuadPart/llA.QuadPart);
	//tp->tv_usec = (int)(((long double)(llB.QuadPart-(long long)tp->tv_sec)*llA.QuadPart)*((long double)1000000.0/(long double)llA.QuadPart));
	tp->tv_usec = (llB.QuadPart * 1000000ull / llA.QuadPart) % 1000000ull;
	
	return 0;
}
#endif

#ifdef WINDOWS
char *basename(const char *filename)
{
	char *ret;
	for (ret = (char *)filename+strlen(filename)-1;*ret!='\\' && *ret!='/' && ret>filename;ret--);
	return (*ret == '\\' || *ret == '/') ? ret+1 : ret;
}
#endif

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
        int32_t  RLoop(uint32_t iInLen, string iInName);
};    

#define MBUFSIZE 8192*10

#ifdef WINDOWS
int truncate(char *filename, size_t off)
{
	int fd;
	fd = _open(filename, _O_CREAT | _O_RDWR);
	_chsize(fd, off);
	_close(fd);
	return 0;
}
#endif

int32_t TestRecv::RLoop(uint32_t iInLen, string iInName)
{
	//blah
	
	clock_t  clFirst, clSecond;
	uint32_t ilSince;
	double   dlFinal;
	
	// poop
	
	struct timeval tlFirst, tlSecond, tlBeg;
	
	fstream  flFile;
	uint32_t ilCurLen;
	uint32_t ilCurBuf;
	uint32_t ilMyBuf;
	int32_t  ilRetf;
	vector<uint8_t> ilDat;
	uint8_t  *vlDat;
	
	//flFile.open(basename(iInName.c_str()), fstream::out | fstream::binary);
	//truncate("testo.png", iInLen);
	
	flFile.open(basename(iInName.c_str()), fstream::out | fstream::binary | fstream::trunc);
	flFile.seekp(iInLen-1, ios_base::beg);
	flFile.write("", 1);
	flFile.flush();
	flFile.seekp(0, ios_base::beg);
	ilDat.resize(MBUFSIZE);
	
	vlDat = &ilDat[0];
	
	//clFirst = clock();
	gettimeofday(&tlFirst, NULL);
	tlBeg = tlFirst;
	ilSince = 0;
	
	for (ilCurBuf=0;ilCurBuf<iInLen;)
	{
		//blah
		
		if ((iInLen-ilCurBuf) < MBUFSIZE)
		    ilCurLen = iInLen-ilCurBuf;
		else
		    ilCurLen = MBUFSIZE;
        
        ilMyBuf = 0;
        
        while (ilMyBuf < ilCurLen)
        	{
        	    //ilRetf = sServer.Recv((char *)&ilDat[ilMyBuf], ilCurLen-ilMyBuf);
        	    ilRetf = sServer.Recv((char *)vlDat+ilMyBuf, ilCurLen-ilMyBuf);
        	    if (ilRetf > 0)
        	        ilMyBuf += ilRetf;
        	    else if (ilRetf < 0)
        	    {
        	    	#ifdef UNIX
        	    	if (errno != EWOULDBLOCK && errno != EINTR)
        	    	    return ilCurBuf+ilMyBuf;
        	    	#elif WINDOWS
        	    	if (WSAGetLastError() != WSAEINPROGRESS && WSAGetLastError() != WSAEWOULDBLOCK && WSAGetLastError() != WSAEMSGSIZE && WSAGetLastError() != WSAEINTR)
        	    	    return ilCurBuf+ilMyBuf;
        	    	#endif
        	    }
        	}
        //flFile.write((char *)&ilDat[0], ilCurLen);
        flFile.write((char *)vlDat, ilCurLen);
        ilCurBuf += ilCurLen;
        
        gettimeofday(&tlSecond, NULL);
        
        if (tlSecond.tv_sec-tlFirst.tv_sec) {
        	
            dlFinal = (double)(tlSecond.tv_sec-tlFirst.tv_sec)+(double)(tlSecond.tv_usec-tlFirst.tv_usec)/1000000;
            
            dlFinal = (((double)ilCurBuf-(double)ilSince)/1024.0)/dlFinal;
            
            erase();
            mvprintw(0, 0, "Speed:        %06.2f KB/s", dlFinal);
            mvprintw(1, 0, "Progress      %02.1f%%", ((double)ilCurBuf/(double)iInLen)*100.0);
		    mvprintw(2, 0, "Time Elapsed: %02u:%02u", (tlSecond.tv_sec-tlBeg.tv_sec)/60, ((tlSecond.tv_sec-tlBeg.tv_sec)-((tlSecond.tv_sec-tlBeg.tv_sec)/60)*60));
            refresh();
            
            tlFirst.tv_sec  = tlSecond.tv_sec;
			tlFirst.tv_usec = tlSecond.tv_usec;
        	ilSince = ilCurBuf;
        }
		/*
        erase();
        mvprintw(0, 0, "Speed:        %06.2f KB/s", dlFinal);
        mvprintw(1, 0, "Progress      %02.1f%%", ((double)ilCurBuf/(double)iInLen)*100.0);
		mvprintw(2, 0, "Time Elapsed: %02u:%02u", (tlSecond.tv_sec-tlBeg.tv_sec)/60, ((tlSecond.tv_sec-tlBeg.tv_sec)-((tlSecond.tv_sec-tlBeg.tv_sec)/60)*60));
        refresh();
        */
	}
	
	
	dlFinal = (double)(tlSecond.tv_sec-tlBeg.tv_sec)+(double)(tlSecond.tv_usec-tlBeg.tv_usec)/1000000;
	dlFinal = (iInLen/1024.0)/dlFinal;
	
	erase();
	mvprintw(0, 0, "Speed:        %06.2f KB/s", dlFinal);
	mvprintw(1, 0, "Progress      %02.1f%%", ((double)ilCurBuf/(double)iInLen)*100.0);
	mvprintw(2, 0, "Time Elapsed: %02u:%02u", (tlSecond.tv_sec-tlBeg.tv_sec)/60, ((tlSecond.tv_sec-tlBeg.tv_sec)-((tlSecond.tv_sec-tlBeg.tv_sec)/60)*60));
	refresh();
	
	//clFirst = clSecond;
	ilSince = ilCurBuf;
	
	
	flFile.close();
	
	return 0;
}


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
        	    //printf("                 \r");
        	    //printf("%u/%u\r", ilProgress, iLen);
        	}
        	//printf("\n");
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
    //vector<uint8_t> data;
    
    sServer.RecvT((void *)&iDatLen, 4);
    sServer.RecvT((void *)&iStrLen, 4);
    filename.resize(iStrLen);
    //data.resize(iDatLen);
    sServer.RecvT((void *)filename.c_str(), iStrLen);
    //sServer.RecvX((void *)&data[0], iDatLen);
    
    //out.open("test.png", fstream::binary | fstream::out);
    
    RLoop(iDatLen, filename);
    
    //out.write((char *)(&data[0]), iDatLen);
    //data.resize(0);
    
    mvprintw(3, 0, "Length:  %u\nName:    %s\nNameLen: %u\n", iDatLen, basename(filename.c_str()), iStrLen);
    refresh();
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
    WINDOW *plWin;
    
    
    lFuck.l_onoff = 0;
    lFuck.l_linger = 1;
    
    memset(dat, 0, 11);
    
    plWin = initscr();
    
    Socket::Init();
    
    tsA.Create(50500);
    tsA.Accept(tsR);
    tsA.sListen.Close();
    
    tsR.Recv();
    //cout << errno << endl;
    tsR.sServer.Close();
    
    refresh();
    getch();
    endwin();
    
    Socket::Fini();
    return 0;
}

    
