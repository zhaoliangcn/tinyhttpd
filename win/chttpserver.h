#ifndef CHTTPSERVER_H
#define CHTTPSERVER_H
#include <winsock2.h>
#include <process.h>
#include <memory.h>
#include <string>
#include "chttpserver.h"
#include "debug.h"
#include "chttprequestqueue.h"
#include "cmemeorycache.h" 
#include "util.h" 
#include <string>
#include <map>
typedef std::map<string, string>				MIMETYPES;
#define HTTPVERSION11  "HTTP/1.1"
#define HTTP_STATUS_CODE_200   200
#define HTTP_STATUS_MSG_200    "OK"
#define HTTP_STATUS_CODE_404   404
#define HTTP_STATUS_MSG_404    "Not Found"
#define HTTP_HEADER_CTYPE 		"Content-Type"
#define HTTP_HEADER_CLENGTH		"Content-Length"
/*
 * No description
 */
class ChttpServer
{
	// private section
	public:
		// class constructor
		ChttpServer();
		// class destructor
		~ChttpServer();
	public:
		void InitMime();
		BOOL InitSock();
		BOOL SockLoop();
		void CloseSock();
		virtual void ParseRequest();
		BOOL ProcessGet(PHTTPREQUEST req);
		BOOL ProcessPost(PHTTPREQUEST req); 
		static unsigned __stdcall SockWorkingThreadFunc(void * param);	
		BOOL StartWorkingThread();
		BOOL TermWorkingThread();
		void SetServerRoot(string root);
		const char * getserverroot();
		void GetFileMimeType(char * filename,std::string &mimetype);
	private:    
		SOCKET mysock;
		bool closesock;
		char buf[2048];		
		CMemeoryCache mymemcache;
		CHttpRequestQueue requestqueue;
		HANDLE   hth1;
    	unsigned  uiThread1ID;
    	HANDLE sockevent;
    	bool working;    	
    	string httpserverroot;
		MIMETYPES		MimeTypes;
};

#endif // CHTTPSERVER_H

