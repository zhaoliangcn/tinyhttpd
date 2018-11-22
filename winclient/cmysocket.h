#ifndef CMYSOCKET_H
#define CMYSOCKET_H
#include <winsock2.h>
#include <process.h>
#include <memory.h>
#include "debug.h"
class CMySocket
{
	// private section
	public:
		// class constructor
		CMySocket();
		// class destructor
		~CMySocket();
		BOOL InitSock();
		void CloseSock();
		static unsigned __stdcall SockWorkingThreadFunc(void * param);	
		BOOL StartWorkingThread();
		BOOL TermWorkingThread();
		void *(ThreadCallBack)(void * param);
		void ThreadCoreFunc();
		HANDLE   hth1;
	protected:
	private:
		SOCKET mysock;
		bool closesock;
		char buf[2048];
    	unsigned  uiThread1ID;
    	HANDLE sockevent;
    	bool working;
};

#endif // CMYSOCKET_H

