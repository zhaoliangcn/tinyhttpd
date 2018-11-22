#ifndef CHTTPREQUESTQUEUE_H
#define CHTTPREQUESTQUEUE_H
#include <windows.h>
#include <queue>
#include <vector>
using namespace std;

#define REQUEST_GET (0)
#define REQUEST_POST (1) 

typedef struct _tag_httpheader
{
	char headername[128];
	char headercontent[256];
	_tag_httpheader * NextHeader;
}HTTPHEADER,*PHTTPHEADER;

typedef struct _tag_httprequest
{
	char request[4096];
	int requestlen;
	SOCKET  clientsocket;
	BYTE requesttype;
	HTTPHEADER * headers;
}HTTPREQUEST,*PHTTPREQUEST;

typedef struct _tag_httpresponse
{
	HTTPHEADER * headers;
	SOCKET  clientsocket;
	char * response_body;
	int response_body_length;
}HTTPRESPONSE,*PHTTPRESPONSE;

typedef queue<PHTTPREQUEST> queueHTTPREQUEST;
/*
 * No description
 */
class CHttpRequestQueue
{
	// private section
	public:
		// class constructor
		CHttpRequestQueue();
		// class destructor
		~CHttpRequestQueue();
		PHTTPREQUEST NewRequest();
		void DeleteRequest(PHTTPREQUEST request);
		void AddRequest(PHTTPREQUEST request);
		PHTTPREQUEST GetRequest();
		int GetSize();
		void DestroyAll();
	protected:
		CRITICAL_SECTION cs;
		CRITICAL_SECTION cs2;
		queueHTTPREQUEST queueRequest;
		queueHTTPREQUEST queueRequestunused;
		
};

#endif // CHTTPREQUESTQUEUE_H

