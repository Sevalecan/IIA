#define IIASOURCE
#include <threads/Thread.hpp>

namespace IIALib
{
	// IIALib
    
    namespace Threads
    {
    	// Threads
    	
    	Thread::Thread()
    	{
    		iFlags = 0;
    	}
    	
    	Thread::~Thread()
    	{
    		void **plStatus;
    		int32_t ilRet;
    		
    		if (iFlags & TF_EXIST)
            	ilRet = Join(plStatus);
    	}
    	
    	int32_t Thread::Create(void *(*plStartRoutine)(void *), void *plArg)
    	{
    		int32_t ilRet;
    		
    		if (iFlags & TF_EXIST)
    		    return -1;
            
            ThreadArg = plStartRoutine;
            
    		ilRet = pthread_create(&cThread, NULL, Thread::ThreadCallC, (void *)this);
    		if (ilRet == 0)
    		    iFlags |= TF_EXIST;
            
            return ilRet;
    	}
    	
    	int32_t Thread::Create(pthread_attr_t &clThreadAttr, void *(*plStartRoutine)(void *), void *plArg)
    	{
    		int32_t ilRet;
    		
    		if (iFlags & TF_EXIST)
                return -1;
            
            ThreadArg = plStartRoutine;
            
            ilRet = pthread_create(&cThread, &clThreadAttr, Thread::ThreadCallC, (void *)this);
            if (ilRet == 0)
                iFlags |= TF_EXIST;
            
            return ilRet;
    	}
    	
        int32_t Thread::Create(void *plArg)
    	{
    		int32_t ilRet;
    		
    		if (iFlags & TF_EXIST)
    		    return -1;
            
    		ilRet = pthread_create(&cThread, NULL, Thread::ThreadCall, (void *)this);
    		if (ilRet == 0)
    		    iFlags |= TF_EXIST;
            
            return ilRet;
    	}
    	
    	int32_t Thread::Create(pthread_attr_t &clThreadAttr, void *plArg)
    	{
    		int32_t ilRet;
    		
    		if (iFlags & TF_EXIST)
                return -1;
            
            ilRet = pthread_create(&cThread, &clThreadAttr, Thread::ThreadCall, (void *)this);
            if (ilRet == 0)
                iFlags |= TF_EXIST;
            
            return ilRet;
    	}
    	
    	
    	int32_t Thread::Exists()
    	{
    		return iFlags & TF_EXIST;
    	}
    	
    	
    	int32_t Thread::Detach()
    	{
    		return pthread_detach(cThread);
    	}
    	
    	void Thread::Exit(void *plStatus)
    	{
    		return pthread_exit(plStatus);
    	}
    	
    	
    	int32_t Thread::GetSchedParam(int *plPolicy, struct sched_param *plParam)
    	{
    		return pthread_getschedparam(cThread, plPolicy, plParam);
    	}
    	
    	
    	int32_t Thread::Join(void **plStatus)
    	{
    		return pthread_join(cThread, plStatus);
    		return pthread_join(cThread, plStatus);
    	}
    	
    	pthread_t Thread::Self()
    	{
    		return pthread_self();
    	}
    	
    	int32_t Thread::SetSchedParam(int32_t iPolicy, const struct sched_param *plParam)
    	{
    		return pthread_setschedparam(cThread, iPolicy, plParam);
    	}
    	
    	int32_t Thread::Yield()
    	{
    		return sched_yield();
    	}
    	
    	int32_t Thread::Cancel()
    	{
    		return pthread_cancel(cThread);
    	}
    	
    	int32_t Thread::operator ==(Thread &clThread)
    	{
    		return pthread_equal(cThread, clThread.cThread);
    	}
    	
    	void *Thread::ThreadMain(void *plArg)
    	{
    		return NULL;
    	}
    	
    	
    	void *Thread::ThreadCall(void *plThread)
    	{
    		void *plRet;
    		Thread *pxThread = (Thread *)plThread;
    		
    		plRet = pxThread->ThreadMain(pxThread->pArg);
    		
    		pxThread->iFlags &= ~TF_EXIST;
    		return plRet;
    	}
    	
        void *Thread::ThreadCallC(void *plThread)
        {
        	void *plRet;
    		Thread *pxThread = (Thread *)plThread;
    		
    		plRet = pxThread->ThreadArg(plThread);
    		
    		pxThread->iFlags &= ~TF_EXIST;
    		return plRet;
        }
    	
        /*
    	int32_t Thread::JoinNP(void **plStatus)
    	{
    		return pthread_join_np(cThread, plStatus);
    	}
    	*/
    	
        /*
    	pthread_id_np_t Thread::GetThreadID()
    	{
    		return pthread_getthreadid_np();
    	}
    	
    	int32_t Thread::GetUnique(pthread_id_np_t *pID)
    	{
    		return pthread_getunique_np(cThread, pID);
    	}
    	*/
    	
        /*
    	int32_t Thread::ExtendedJoin(void **plStatus, pthread_joinoption_np_t *plOptions)
    	{
    		return pthread_extendedjoin_np(cThread, void **status, pthread_joinoption_np_t *options);
    	}
    	*/
    	
        /*
    	int32_t Thread::Delay(const struct timespec *plInterval)
    	{
    		return pthread_delay_np(plInterval);
    	}
    	*/
    	
    }
    
}
