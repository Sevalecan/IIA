#include "filesend.hpp"

using namespace std;
using namespace IIALib::Socket;

namespace FileSend
{
	// blah
	
	int32_t FSSession::Connect(string slHost, int16_t ilPort)
    {
        SockAddrIn slAddr;
        HostEnt    hlHost;
        
        hlHost.GetHostByName(slHost);
        
        slAddr.saFamily = AF_INET;
        slAddr.sinAddr.sAddr = *(uint32_t *)(hlHost.hAddrList[0].iData)
        slAddr.sinPort = htons(ilPort);
        
        sPeer.Connect(slAddr);
        
        return 0;
    }
	
    int32_t FSSession::Listen(int16_t ilPort)
    {
        SockAddrIn slAddr;
        Socket     slAccp;
        
        slAddr.saFamily      = AF_INET;
        slAddr.sinAddr.sAddr = INADDR_ANY;
        slAddr.sinPort = htons(ilPort);
        
        sPeer.Bind(&slAddr);
        sPeer.Listen(1);
        sPeer.Accept(slAccp);
        
        sPeer.Close();
        sPeer = slAccp;
        
        return 0;
    }
    
    int32_t FSSession::SessionLoop()
    {
    	
    }
	
}