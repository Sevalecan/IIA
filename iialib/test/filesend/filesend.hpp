#ifndef FILESEND_HPP
#define FILESEND_HPP

#include "fsprec.h"
#include <stdint.h>
#include <string>
#include <socket/Socket.hpp>

#define FS_CONNECTED 0x0001;
#define FS_SERVER    0x0002;
#define FS_CLIENT    0x0004;

namespace FileSend
{
	// blah
	
	
	class FSSession
	{
		private:
            
            IIALib::Socket::Socket sPeer;
            uint32_t               iMode;
            std::string            sFileName;
            
		public:
            
            FSSession();
			~FSSession();
            
            int32_t Connect(std::string slHost, int16_t ilPort);
            int32_t Listen(int16_t ilPort);
            
            int32_t SessionLoop();
            
	};
	
	
}


#endif