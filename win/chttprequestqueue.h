#ifndef CHTTPREQUESTQUEUE_H
#define CHTTPREQUESTQUEUE_H
#include <windows.h>
#include <queue>
using namespace std;
typedef struct tag_httprequest
{
	char request[4096];
	int requestlen;
	SOCKET  clientsocket;
}HTTPREQUEST,*PHTTPREQUEST;

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

