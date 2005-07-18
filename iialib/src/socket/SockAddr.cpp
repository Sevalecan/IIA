#include "socket/SockAddr.hpp"
#include "exceptions/Exception.hpp"

using namespace IIALib::Exceptions;

namespace IIALib
{
    
    namespace Socket
    {
        
        SockAddr::saPType::saPType()
        {
            iLength = 0;
            pData = NULL;
        }    
        
        SockAddr::saPType::saPType(uint32_t iInLength)
        {
            Resize(iInLength);
        }    
        
        SockAddr::saPType::~saPType()
        {
            Resize(0);
        }    
        
        uint32_t SockAddr::saPType::Resize(uint32_t iInLength)
        {
            uint8_t *iTemp;
            
            if (iInLength == 0 && iLength > 0 && pData != NULL)
            {
                iLength = 0;
                free(pData);
            }
            else if (iInLength > 0 && iLength == 0) {
                pData = (uint8_t *)malloc(iInLength);
                if (pData) iLength = iInLength;
                else throw Exception("Unable to allocate memory.", E_NOMEMORY, E_MEDIUM);
            }
            else if (iInLength > 0 && iLength > 0 && iInLength != iLength) {
                iTemp = (uint8_t *)realloc(pData, iInLength);
                if (iTemp == NULL) throw Exception("Unable to reallocate memory.", E_NOMEMORY, E_MEDIUM);
                pData = iTemp;
                iLength = iInLength;
            }    
            
            return 0;
        }    
        
        
    }    
    
}

    
