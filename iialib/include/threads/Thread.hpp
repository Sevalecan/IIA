#ifndef THREAD_HPP
#define THREAD_HPP

#include <setup.h>
#include <pthread.h>
#include <stdint.h>

#ifdef WINDOWS
#undef Yield
#endif

namespace IIALib
{
	
    namespace Threads
    {
    	// threads
    	
    	#define TF_EXIST 0x01
    	
    	class IIAEXPORT Thread
    	{
    		public:
                static void *ThreadCall(void *plThread);
                static void *ThreadCallC(void *plThread);
                
                pthread_t      cThread;
                uint32_t       iFlags;
                void          *pArg;
                
                Thread();
                ~Thread();
                
                int32_t Create(void *plArg);
                int32_t Create(pthread_attr_t &clThreadAttr, void *plArg);
                int32_t Create(void *(*plStartRoutine)(void *), void *plArg);
                int32_t Create(pthread_attr_t &clThreadAttr, void *(*plStartRoutine)(void *), void *plArg);
                int32_t Exists();
                
                int32_t Detach();
                void    Exit(void *plStatus);
                int32_t GetSchedParam(int *plPolicy, struct sched_param *plParam);
                int32_t Join(void **plStatus);
                pthread_t Self();
                int32_t SetSchedParam(int32_t iPolicy, const struct sched_param *plParam);
                int32_t Yield();
                int32_t Cancel();
                int32_t operator ==(Thread &clThread);
                //int32_t JoinNP(void **plStatus);
                //pthread_id_np_t GetThreadID();
                //int32_t GetUnique(pthread_id_np_t *pID);
                //int32_t ExtendedJoin(void **plStatus, pthread_joinoption_np_t *plOptions);
                //int32_t Delay(const struct timespec *plInterval);
                
                virtual void IIACALL *ThreadMain(void *plArg);
                void *(*ThreadArg)(void *);
                
    	};
    	
    }
    
}

#endif
