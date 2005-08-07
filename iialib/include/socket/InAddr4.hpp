#ifndef INADDR4_HPP
#define INADDR4_HPP

#include <setup.h>
#include <stdint.h>

#ifdef WINDOWS
#include <winsock.h>
#endif

#ifdef UNIX
#include <sys/socket.h>
#endif

#include "socket/InAddr.hpp"

namespace IIALib
{
    namespace Socket
    {

        class IIAEXPORT InAddr4 : public InAddr
        {
            public:
                
                uint32_t sAddr;
                
                InAddr4() { iType = AF_INET; };
                
        };
    };
};    

#endif
