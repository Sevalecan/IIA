#include "socket/HostEnt.hpp"

namespace IIALib
{
    
    namespace Socket
    {
        
        uint32_t HostEnt::Fromhostent(hostent *hRet)
        {
            uint32_t iLen, iPos;
            hName = hRet->h_name;
            
            for (iLen=0;hRet->h_aliases[iLen];iLen++);
            hAliases.resize(iLen);
            
            for (iPos=0;iPos<iLen;iPos++)
                hAliases[iPos] = hRet->h_aliases[iPos];
            
            hAddrType = hRet->h_addrtype;
            hLength = hRet->h_length;
            
            for (iLen=0;hRet->h_addr_list[iLen];iLen++);
            hAddrList.resize(iLen);
            
            for (iPos=0;iPos<iLen;iPos++)
                memcpy(hAddrList[iPos].iData, hRet->h_addr_list[iPos], hLength);
            
            hAddr = (uint8_t *)&hAddrList[0];
            
            return 0;
        }    
        
        uint32_t HostEnt::GetHostByName(std::string sName)
        {
            hostent *hRet;
            
            hRet = gethostbyname(sName.c_str());
            if (hRet == NULL) return 1;
            
            Fromhostent(hRet);
            
            return 0;
        }
            
        uint32_t HostEnt::GetHostByAddr(InAddr *aAddr, uint32_t iLength, uint32_t iFormat)
        {
            hostent *hRet;
            
            hRet = gethostbyaddr((const char *)aAddr->iData, iLength, iFormat);
            
            if (hRet == NULL) return 1;
            
            Fromhostent(hRet);
            
            return 0;
        }
        
    };    
    
};    
