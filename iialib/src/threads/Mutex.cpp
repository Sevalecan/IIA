#define IIASOURCE
#include "threads/Mutex.hpp"

namespace IIALib
{
	// IIALib
    
    namespace Threads
    {
    	// threads
    	
    	Mutex::Mutex()
    	{
    		pthread_mutexattr_init(&cMutexAttr);
    		pthread_mutex_init(&cMutex, &cMutexAttr);
    	}
    	
    	Mutex::Mutex(pthread_mutexattr_t &clMutexAttr)
    	{
    		cMutexAttr = clMutexAttr;
    		pthread_mutex_init(&cMutex, &cMutexAttr);
    	}
    	
    	Mutex::~Mutex()
    	{
    		    Destroy();
    	}
    	
    	int32_t Mutex::Init()
    	{
    		int32_t ilRet;
    		
    		Destroy();
    		pthread_mutexattr_init(&cMutexAttr);
    		ilRet = pthread_mutex_init(&cMutex, &cMutexAttr);
    		
    		return ilRet;
    	}
    	
    	int32_t Mutex::Init(pthread_mutexattr_t &clMutexAttr)
    	{
    		int32_t ilRet;
            
    		Destroy();
    		cMutexAttr = clMutexAttr;
    		ilRet = pthread_mutex_init(&cMutex, &cMutexAttr);
    		
    		return ilRet;
    	}
    	
    	int32_t Mutex::Destroy()
    	{
    		pthread_mutex_destroy(&cMutex);
    		pthread_mutexattr_destroy(&cMutexAttr);
    		
    		return 0;
    	}
    	
    	int32_t Mutex::Lock()
    	{
    		return pthread_mutex_lock(&cMutex);
    	}
    	
    	int32_t Mutex::TryLock()
    	{
    		return pthread_mutex_trylock(&cMutex);
    	}
    	
    	int32_t Mutex::UnLock()
    	{
    		return pthread_mutex_unlock(&cMutex);
    	}
    	
    	/*
        int32_t Mutex::TimedLock(const struct timespec *cDeltaTime)
    	{
    		return pthread_mutex_timedlock_np(&cMutex, cDeltaTime);
    	}
    	*/
    	
    }
    
}
