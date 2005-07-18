#include "socket/Socket.hpp"
#include <iostream>


using namespace IIALib::Socket;

int main(int argc, char *argv[]) {
    HostEnt f;
    
    if (argc < 2) return 0;
    
    f.GetHostByName(argv[1]);
    
    for (int x=0;x<f.hAddrList.size();x++) {
        printf("%u.%u.%u.%u\n", (*(uint32_t *)&(f.hAddrList[x]) & 0xFF), (*(uint32_t *)&(f.hAddrList[x]) >> 8 & 0xFF), (*(uint32_t *)&(f.hAddrList[x]) >> 16 & 0xFF), (*(uint32_t *)&(f.hAddrList[x]) >> 24 & 0xFF));
    }    
    printf("Aliases: %u\r\n", f.hAliases.size());
    for (int x=0;x<f.hAliases.size();x++) {
        std::cout << f.hAliases[x];
    }    
    
    return 0;
}

    
