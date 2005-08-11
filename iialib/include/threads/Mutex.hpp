#ifndef MUTEX_HPP
#define MUTEX_HPP

#include <setup.h>
#include <pthread.h>
#include <stdint.h>

namespace IIALib
{
	
    namespace Threads
    {
    	// threads
    	
    	
    	class IIAEXPORT Mutex
    	{
    		public:
                pthread_mutex_t     cMutex;
                pthread_mutexattr_t cMutexAttr;
                int32_t             iFlags;
                
                Mutex();
                Mutex(pthread_mutexattr_t &clMutexAttr);
                ~Mutex();
                
                int32_t Init();
                int32_t Init(pthread_mutexattr_t &clMutexAttr);
                
                int32_t Lock();
                int32_t TryLock();
                int32_t UnLock();
                //int32_t TimedLock(const struct timespec *cDeltaTime);
                
            private:
                int32_t Destroy();
                
    	};
    	
    }
    
}

#endif
