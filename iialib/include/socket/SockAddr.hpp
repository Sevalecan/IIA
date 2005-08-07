#ifndef SOCKADDR_HPP
#define SOCKADDR_HPP

#include <setup.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef WINDOWS
#include <winsock.h>
typedef uint16_t sa_family_t;
#endif

#ifdef UNIX
#include <sys/socket.h>
#endif

namespace IIALib
{
    
    namespace Socket
    {
        
        class IIAEXPORT SockAddr
        {
            public:
                uint16_t saFamily;
                //uint8_t  saData[14];
                uint8_t  saData[0];
                
                class saPType // *sigh* dont ask....
                {
                    public:
                        saPType();
                        saPType(uint32_t iInLength);
                        ~saPType();
                        
                        uint32_t Resize(uint32_t iInLength);
                        
                        uint32_t   iLength;
                        uint8_t   *pData;
                        
                        uint32_t operator=(saPType saInObj)
                        {
                            uint32_t iRet;
                            
                            iRet = Resize(saInObj.iLength);
                            if (iRet != 0) return 1;
                            
                            iLength = saInObj.iLength;
                            memcpy((void *)pData, (void *)saInObj.pData, iLength);
                            
                            return iRet;
                        }    
                        
                };    
                
                virtual saPType IIACALL GetStdSockAddr();
                
        };    
        
    };    
    
};    


#endif
