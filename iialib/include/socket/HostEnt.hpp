#ifndef HOSTENT_HPP
#define HOSTENT_HPP

#include <stdint.h>
#include <vector>
#include <string>

#ifdef WINDOWS
#include <winsock.h>
#endif

#ifdef UNIX
#include <sys/socket.h>
#include <netdb.h>
#endif

#include "socket/InAddr.hpp"

namespace IIALib
{
    namespace Socket
    {
        
        class HostEnt
        {
            public:
                
                class tByte16
                {
                    public:
                        uint8_t iData[16];
                };    
                
                std::string              hName;
                std::vector<std::string> hAliases;
                uint32_t                 hAddrType;
                uint32_t                 hLength;
                std::vector<tByte16>     hAddrList;
                uint8_t                 *hAddr;
                
                uint32_t GetHostByName(std::string sName);
                uint32_t GetHostByAddr(InAddr *aAddr, uint32_t iLength, uint32_t iFormat);
                
            private:
                
                uint32_t Fromhostent(hostent *hRet);
                
        };   
        
         
    };
    
};    

#endif
