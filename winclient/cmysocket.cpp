#include "cmysocket.h" // class's header file
#include <stdio.h>
// class constructor
CMySocket::CMySocket() {
	sockevent=CreateEvent(NULL,TRUE,FALSE,"sockevent");
	closesock=false;
	working=true;
	InitSock();
}

// class destructor
CMySocket::~CMySocket() {
	CloseHandle(sockevent);
	TermWorkingThread();
	WSACleanup( );
}
BOOL CMySocket::InitSock() {
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested =MAKEWORD( 2, 2 );
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		/* Tell the user that we couldn't find a useable */
		/* winsock.dll. */
		return FALSE;
	}
	/* Confirm that the Windows Sockets DLL supports 1.1.*/
	/* Note that if the DLL supports versions greater */
	/* than 1.1 in addition to 1.1, it will still return */
	/* 1.1 in wVersion since that is the version we */
	/* requested. */
	if ( LOBYTE( wsaData.wVersion ) != 2 ||
	        HIBYTE( wsaData.wVersion ) != 2 ) {
		/* Tell the user that we couldn't find a useable */
		/* winsock.dll. */
		WSACleanup( );
		return FALSE;
	}

	return TRUE;
}
void CMySocket::CloseSock() {
	closesock =true;
	shutdown(mysock,SD_BOTH);
	closesocket(mysock);
}
void CMySocket::ThreadCoreFunc() {
	struct sockaddr_in server_addr;
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(80);
	server_addr.sin_addr.s_addr = inet_addr("192.168.0.244");
	memset(&(server_addr.sin_zero),0,8);

	char requestbuf[512];
	char responsebuf[32768];
	memset(requestbuf,0,sizeof(requestbuf));
	memset(responsebuf,0,sizeof(responsebuf));
	sprintf(requestbuf,"GET / HTTP/1.1\r\n\r\n");


	while(true) {
		if(!working)
			break;
		mysock=socket(AF_INET,SOCK_STREAM,0);

		if (connect(mysock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
			perror("connect fail");
			continue;
		}
		for(int i=0; i<10000; i++) {

			if(send(mysock,requestbuf,strlen(requestbuf),0)>0) {
				if(recv(mysock,responsebuf,sizeof(responsebuf),0)>0) {
					_DbgPrint("%s",responsebuf);
					break;
				}
			} else
				break;
		}
		shutdown(mysock,SD_BOTH);
		closesocket(mysock);

	}
	return ;
}
unsigned __stdcall CMySocket::SockWorkingThreadFunc(void * param) {
	CMySocket * pSock=(CMySocket *)param;

	pSock->ThreadCoreFunc();

	return 0;

}
BOOL CMySocket::StartWorkingThread() {
	hth1 = (HANDLE)_beginthreadex( NULL,         // security
	                               0,            // stack size
	                               SockWorkingThreadFunc,
	                               (void *)this,           // arg list
	                               CREATE_SUSPENDED,  // so we can later call ResumeThread()
	                               &uiThread1ID );

	if ( hth1 == 0 )
		_DbgPrint("Failed to create thread 2\n");
	working=true;
	DWORD   dwExitCode;
	GetExitCodeThread( hth1, &dwExitCode );  // should be STILL_ACTIVE = 0x00000103 = 259
	_DbgPrint( "initial thread 3 exit code %d",dwExitCode );
	ResumeThread( hth1 );
	return TRUE;
}
BOOL CMySocket::TermWorkingThread() {
	DWORD   dwExitCode;
	working=false;
	GetExitCodeThread( hth1, &dwExitCode );
	_DbgPrint( "thread 3 exited with code %d" ,dwExitCode);
	CloseHandle( hth1 );
	return TRUE;
}
