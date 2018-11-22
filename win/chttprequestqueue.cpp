#include "chttprequestqueue.h" // class's header file

// class constructor
CHttpRequestQueue::CHttpRequestQueue()
{
	InitializeCriticalSection(&cs);
	InitializeCriticalSection(&cs2);
}

// class destructor
CHttpRequestQueue::~CHttpRequestQueue()
{
	DeleteCriticalSection(&cs);
	DeleteCriticalSection(&cs2);
	DestroyAll();
}
PHTTPREQUEST CHttpRequestQueue::NewRequest()
{
	PHTTPREQUEST ptemp;
	EnterCriticalSection(&cs2);
	if(queueRequestunused.size()>0)
	{
		ptemp=queueRequestunused.front();
		queueRequestunused.pop();
	}
	else
	{
		ptemp=new HTTPREQUEST();
		
	}
	LeaveCriticalSection(&cs2);
	return ptemp;
}
void CHttpRequestQueue::DeleteRequest(PHTTPREQUEST request)
{
	EnterCriticalSection(&cs2);
	queueRequestunused.push(request);
	LeaveCriticalSection(&cs2);
	return ;
}
void CHttpRequestQueue::AddRequest(PHTTPREQUEST request)
{
	EnterCriticalSection(&cs);
	queueRequest.push(request);
	LeaveCriticalSection(&cs);
}
PHTTPREQUEST CHttpRequestQueue::GetRequest()
{
	PHTTPREQUEST tmp;
	EnterCriticalSection(&cs);	
	tmp= queueRequest.front();
	queueRequest.pop();
	LeaveCriticalSection(&cs);
	return tmp;

}
int CHttpRequestQueue::GetSize()
{
	EnterCriticalSection(&cs);
	int size = queueRequest.size();
	LeaveCriticalSection(&cs);
	return size;
}
void CHttpRequestQueue::DestroyAll()
{
	PHTTPREQUEST ptemp;
	while(queueRequest.size()>0)
	{
		ptemp=queueRequest.front();
		queueRequest.pop();
		delete ptemp;
	}
	while(queueRequestunused.size()>0)
	{
		ptemp=queueRequestunused.front();
		queueRequestunused.pop();
		delete ptemp;
	}
	return ;
}
