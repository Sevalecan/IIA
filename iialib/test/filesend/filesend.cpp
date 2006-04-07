#include "filesend.hpp"

using namespace std;
using namespace IIALib::Socket;

namespace FileSend
{
	// blah
	
	FSSession::FSSession()
	{
		iMode = 0;
	}
	
	int32_t FSSession::Connect(string slHost, int16_t ilPort)
    {
    	uint32_t   ilRet;
        SockAddrIn slAddr;
        HostEnt    hlHost;
        
        hlHost.GetHostByName(slHost);
        
        slAddr.saFamily = AF_INET;
        slAddr.sinAddr.sAddr = *(uint32_t *)(hlHost.hAddrList[0].iData)
        slAddr.sinPort = htons(ilPort);
        
        ilRet = sPeer.Connect(slAddr);
        if (ilRet != 0)
            return -1;
        
        return 0;
    }
	
    int32_t FSSession::Listen(int16_t ilPort)
    {
    	uint32_t   ilRet;
        SockAddrIn slAddr;
        Socket     slAccp;
        
        slAddr.saFamily      = AF_INET;
        slAddr.sinAddr.sAddr = INADDR_ANY;
        slAddr.sinPort = htons(ilPort);
        
        ilRet = sPeer.Bind(&slAddr);
        if (ilRet) throw Exception("Could not bind to any address.", errno);
        
        ilRet = sPeer.Listen(5);
        if (ilRet) throw Exception("Unable to listen for connections.", errno);
        
        ilRet = sPeer.Accept(slAccp);
        if (ilRet) throw Exception("Error accepting connection.", errno);
        
        sPeer.Close();
        sPeer = slAccp;
        
        return 0;
    }
    
    int32_t FSSession::SessionLoop()
    {
    	FSPacket cPacket;
    	cPacket.vData.resize(8192);
    	
    	
    	
    	return 0;
    }
	
}