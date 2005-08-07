#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <socket/Socket.hpp>
#include <fstream>
#include <time.h>
#include <curses.h>
#ifdef UNIX
#include <sys/time.h>
#endif

using namespace std;
using namespace IIALib::Socket;


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

class TestSend
{
	public:
	    Socket sSock;
	    void Send(string filename);
	    void Connect(string dest);
	    int32_t SLoop(fstream &iInFile, uint32_t iInLen);
};

void TestSend::Connect(string dest)
{
	SockAddrIn v;
	HostEnt   he;
	
	he.GetHostByName(dest);
	v.sinAddr.sAddr = *(uint32_t *)(he.hAddrList[0].iData);
	v.sinPort = htons(50500);
	v.saFamily = AF_INET;
	
	sSock.Create();
	sSock.Connect(v);
	
	return;
}

void TestSend::Send(string filename)
{
	fstream out;
	uint32_t len;
	uint32_t loc;
	uint32_t tmp;
	vector<uint8_t> dat;
	
	out.open(filename.c_str(), fstream::in | fstream::binary | fstream::out);
	
	out.seekg(0, ios::end);
	len = (uint32_t)out.tellg();
	out.seekg(0, ios::beg);
	
	//dat.resize(8*1024);
	//out.read((char *)(&dat[0]), len);
	
	tmp = filename.size()+1;
	
	sSock.SendT(&len, 4);
	sSock.SendT(&tmp, 4);
	sSock.SendT((void *)filename.c_str(), tmp);
	
	loc = 0;
	
	SLoop(out, len);
	
	dat.resize(0);
	
	out.close();
	return;
}

#define MBUFSIZE 8192*10



int32_t TestSend::SLoop(fstream &iInFile, uint32_t iInLen)
{
	clock_t  clFirst, clSecond, clFinal;
	double   dlFinal;
	
	struct timeval tlFirst, tlSecond;
	struct timeval tlBeg;
	
	uint32_t ilBufLen;
	uint32_t ilCurLen;
	uint32_t ilCurBuf;
	uint32_t ilSince;
	uint32_t ilRetf;
	int32_t  ilError;
	vector<uint8_t> vlDat;
	uint8_t  *alDat;
	
	ilSince = 0;
	
	vlDat.resize(MBUFSIZE);
	alDat = &vlDat[0];
	
	gettimeofday(&tlFirst, NULL);
	tlBeg = tlFirst;
	
	for (ilCurBuf=0;ilCurBuf<iInLen;)
	{
		// blargh
        if ((iInLen-ilCurBuf) < MBUFSIZE)
            ilCurLen = iInLen-ilCurBuf;
        else
            ilCurLen = MBUFSIZE;
        
        ilBufLen = 0;
        
        //iInFile.read((char *)&vlDat[0], ilCurLen);
        iInFile.read((char *)alDat, ilCurLen);
        
        while (ilBufLen < ilCurLen)
        {
            //ilRetf = sSock.Send((char *)(&vlDat[0]+ilBufLen), ilCurLen-ilBufLen);
            ilRetf = sSock.Send(((char *)alDat)+ilBufLen, ilCurLen-ilBufLen);
            if (ilRetf > 0)
                ilBufLen += ilRetf;
            else
            {
                #ifdef UNIX
                if (errno != EWOULDBLOCK && errno != ENOBUFS && errno != EINTR)
                    return ilCurBuf + ilBufLen;
                #elif WINDOWS
                ilError = WSAGetLastError();
                if (ilError != WSAEINPROGRESS && ilError != WSAEWOULDBLOCK && ilError != WSAEMSGSIZE && ilError != WSAEINTR)
                    return ilCurBuf + ilBufLen;
                #endif
            }
        }
        
        ilCurBuf += ilCurLen;
        
        gettimeofday(&tlSecond, NULL);
	    
        if (tlSecond.tv_sec-tlFirst.tv_sec)
        {
            dlFinal = (double)(tlSecond.tv_sec-tlFirst.tv_sec)+(double)(tlSecond.tv_usec-tlFirst.tv_usec)/1000000;
            
            dlFinal = (((double)ilCurBuf-(double)ilSince)/1024.0)/dlFinal;
			
			erase();
            mvprintw(0, 0, "Speed:        %06.2f KB/s", dlFinal);
            mvprintw(1, 0, "Progress:     %02.1f%%", ((double)ilCurBuf/(double)iInLen)*100.0);
		    mvprintw(2, 0, "Time Elapsed: %02u:%02u", (tlSecond.tv_sec-tlBeg.tv_sec)/60, ((tlSecond.tv_sec-tlBeg.tv_sec)-((tlSecond.tv_sec-tlBeg.tv_sec)/60)*60));
            refresh();
			
			tlFirst.tv_sec  = tlSecond.tv_sec;
			tlFirst.tv_usec = tlSecond.tv_usec;
			
			ilSince = ilCurBuf;
        }
		
		/*
        erase();
        mvprintw(0, 0, "Speed:        %06.2f KB/s", dlFinal);
        mvprintw(1, 0, "Progress:     %02.1f%%", ((double)ilCurBuf/(double)iInLen)*100.0);
		mvprintw(2, 0, "Time Elapsed: %02u:%02u", (tlSecond.tv_sec-tlBeg.tv_sec)/60, ((tlSecond.tv_sec-tlBeg.tv_sec)-((tlSecond.tv_sec-tlBeg.tv_sec)/60)*60));
        refresh();
        */
	}
	
	dlFinal = (double)(tlSecond.tv_sec-tlBeg.tv_sec)+(double)(tlSecond.tv_usec-tlBeg.tv_usec)/1000000;
	dlFinal = (iInLen/1024.0)/dlFinal;
	
	
	erase();
       mvprintw(0, 0, "Speed:        %06.2f KB/s", dlFinal);
       mvprintw(1, 0, "Progress:     %02.1f%%", ((double)ilCurBuf/(double)iInLen)*100.0);
	   mvprintw(2, 0, "Time Elapsed: %02u:%02u", (tlSecond.tv_sec-tlBeg.tv_sec)/60, ((tlSecond.tv_sec-tlBeg.tv_sec)-((tlSecond.tv_sec-tlBeg.tv_sec)/60)*60));
       refresh();
	
	vlDat.resize(0);
	
    return 0;
}

int main(int argc, char *argv[])
{
	TestSend ts;
	string ex;
	WINDOW *plWin;
	
	plWin = initscr();
	
	
	if (argc < 3) return 0;
	
	ex = argv[1];
	
	Socket::Init();
	
	ts.Connect(argv[2]);
	ts.Send(ex.c_str());
	
	ts.sSock.Close();
	
	Socket::Fini();
	
	refresh();
	getch();
	endwin();
	
	return 0;
}
