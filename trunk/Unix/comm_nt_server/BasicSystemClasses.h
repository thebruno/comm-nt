#ifndef BASIC_SYSTEM_CLASSES_H
#define BASIC_SYSTEM_CLASSES_H
#include "stdafx.h"
#include "pthread.h"
#include "semaphore.h"

#include "pthread.h"
#include "semaphore.h"
#include "xutility"


//! for compatibility
class SysHandle{
public:
	SysHandle(){}
	virtual ~SysHandle(){}
};

//! universal semaphore class
class SysSemaphore: public SysHandle{
	sem_t SemID;
    int StartingValue ;
    int MaxValue;
public:
        SysSemaphore(int start, int max){
			sem_init(&SemID, 0, start);
            StartingValue = start;
            MaxValue = max;
	}
	void Wait(){
		sem_wait(&SemID);
	}
	void Release(){
		sem_post(&SemID);
	}
	~SysSemaphore(){
		sem_destroy(&SemID);
	}
} * temp;

//! universal thread class
class SysThread: public SysHandle{
	pthread_t ThreadID;	
	std::pair<SysThread*, void*> * toDelete;
public: 
	int ready_to_go;
	pthread_mutex_t suspend_mutex;
	pthread_cond_t suspend_cond;
	bool Exit;
	void* (* Function) (void *arg);
	static void *BasicFunction(void *arg) {		
		std::pair<SysThread*, void*>* params = static_cast<std::pair<SysThread*, void*>*>(arg);		
		SysThread * me = params->first;
		pthread_mutex_lock(&me->suspend_mutex);
		while (me->ready_to_go == 0) {
			pthread_cond_wait(&me->suspend_cond, &me->suspend_mutex);
		}
		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, 0);
		pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, 0);
		pthread_mutex_unlock(&me->suspend_mutex);
		me->Function(params->second);
		return NULL;
	}
  
	SysThread(void* (* f) (void *)): toDelete(0), 
		ready_to_go(0), Exit(false),
		suspend_mutex(PTHREAD_MUTEX_INITIALIZER), 
		suspend_cond(PTHREAD_COND_INITIALIZER){
			Function = f;
			pthread_attr_t attr;
			pthread_attr_init(&attr);
			pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

			std::pair<SysThread*, void*>* params = new std::pair<SysThread*, void*>();
			params->first = this;
			params->second = 0;
			toDelete = params;
			pthread_create(&ThreadID, &attr, BasicFunction, params);
			pthread_attr_destroy(&attr);
	}
	SysThread(void* (* f) (void *), void *arg):toDelete(0),
		ready_to_go(0), Exit(false),
		suspend_mutex(PTHREAD_MUTEX_INITIALIZER), 
		suspend_cond(PTHREAD_COND_INITIALIZER){
			Function = f;
			pthread_attr_t attr;	
			pthread_attr_init(&attr);
			pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

			std::pair<SysThread*, void*>* params = new std::pair<SysThread*, void*>();
			params->first = this;
			params->second = arg;
			toDelete = params;
			pthread_create(&ThreadID, &attr, BasicFunction, params);
			pthread_attr_destroy(&attr);
	}
	void Start(){
			pthread_mutex_lock(&suspend_mutex);
			ready_to_go = 1;
			pthread_mutex_unlock(&suspend_mutex);
			pthread_cond_signal(&suspend_cond);
	}
	void Finish(){
			Exit = true;
	}
	void Join(){
		void * status;
		pthread_join(ThreadID, &status);
	}
	void Terminate(){
		pthread_cancel(ThreadID);
	}
	~SysThread(){
		if (toDelete != 0) {
			if (toDelete->second != 0)
				delete toDelete->second;
			delete toDelete;
			toDelete = 0;
		}
	}
};
#endif
