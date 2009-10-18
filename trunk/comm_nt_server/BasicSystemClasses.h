#ifndef BASIC_SYSTEM_CLASSES_H
#define BASIC_SYSTEM_CLASSES_H

#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#include "stdafx.h"

//! Represents all objects which have handles
class SysHandle{
	HANDLE Handle;
public:
	SysHandle(){
		Handle = INVALID_HANDLE_VALUE;
	}
	SysHandle(HANDLE h):Handle(h){}
	virtual ~SysHandle(){}
	HANDLE GetHandle() {
		return Handle;
	}
	void SetHandle(HANDLE h) {
		Handle = h;
	}
};
//! Windows Thread class
class SysThread: public SysHandle{
	DWORD ThreadID;	
public: 
	bool Exit;
	unsigned long (__stdcall *Function) (void *);
	SysThread(unsigned long (__stdcall *f) (void *)): SysHandle(INVALID_HANDLE_VALUE){
		Function = f;
		Exit = false;
		SetHandle(CreateThread(NULL, 0, Function, 0, CREATE_SUSPENDED, &ThreadID)); 
		if (GetHandle() == INVALID_HANDLE_VALUE || GetHandle() == 0){
			//b³¹d
		}
	}
	SysThread(unsigned long (__stdcall *f) (void *), void * params): SysHandle(INVALID_HANDLE_VALUE){
		Function = f;
		Exit = false;
		SetHandle(CreateThread(NULL, 0, Function, params, CREATE_SUSPENDED, &ThreadID)); 
		if (GetHandle() == INVALID_HANDLE_VALUE || GetHandle() == 0){
			//b³¹d
		}
	}
	void Start(){
		if (GetHandle() != 0 && GetHandle() != INVALID_HANDLE_VALUE){
			ResumeThread(GetHandle());
		}
	}
	void Finish(){
		if (GetHandle() != 0 && GetHandle() != INVALID_HANDLE_VALUE){

			Exit = true;
		}
	}
	void Suspend(){
		if (GetHandle() != 0 && GetHandle() != INVALID_HANDLE_VALUE){
			SuspendThread(GetHandle());
		}
	}
	
	void Join(){
		if (GetHandle() != 0 && GetHandle() != INVALID_HANDLE_VALUE){
			WaitForSingleObject(GetHandle(),INFINITE);
		}
	}
	void Terminate(){
		if (GetHandle() != 0 && GetHandle() != INVALID_HANDLE_VALUE){
			TerminateThread(GetHandle(),1);
		}
	}
	~SysThread(){
		if (GetHandle() != 0 && GetHandle() != INVALID_HANDLE_VALUE){
			CloseHandle(GetHandle());
			SetHandle(0);
		}
	}
};
//! Windows Semaphore class
class SysSemaphore: public SysHandle{
	int StartingValue, MaxValue;
public:
	SysSemaphore(int start, int max): StartingValue(start),MaxValue(max), SysHandle(INVALID_HANDLE_VALUE){
		SetHandle(CreateSemaphore(NULL, start, max, NULL));
		if (GetHandle() == INVALID_HANDLE_VALUE || GetHandle() == 0){
			//b³¹d
		}
	}
	void Wait(){
		if (GetHandle() != 0 && GetHandle() != INVALID_HANDLE_VALUE){
			unsigned long result = WaitForSingleObject(GetHandle(),INFINITE);
			if ( result == WAIT_OBJECT_0){
				//MessageBox(0,L"asdf",L"asdf",0);

			}
		}
	}
	void Release(){
		if (GetHandle() != 0 && GetHandle() != INVALID_HANDLE_VALUE){
			ReleaseSemaphore(GetHandle(),1,NULL);
		}
	}
	~SysSemaphore(){
		if (GetHandle() != 0 && GetHandle() != INVALID_HANDLE_VALUE){
			CloseHandle(GetHandle());
			SetHandle(0);
		}
	}
};

//		SearchThread = new SysThread(FilesSearcherThreadFnc,this);
#endif