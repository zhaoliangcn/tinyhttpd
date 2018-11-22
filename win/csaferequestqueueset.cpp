#include "csaferequestqueueset.h" // class's header file

// class constructor
CSafeRequestQueueSet::CSafeRequestQueueSet()
{
	// insert your code here
}

// class destructor
CSafeRequestQueueSet::~CSafeRequestQueueSet()
{
	// insert your code here
}
CHttpRequestQueue * CSafeRequestQueueSet::MakeNewQueue()
{
	CHttpRequestQueue * temp =new CHttpRequestQueue;
	queueset.push_back(temp);
	return temp; 
}
void CSafeRequestQueueSet::DestroyAll()
{
	while(queueset.size()>0)
	{
		CHttpRequestQueue * temp = queueset.back();
		
		queueset.pop_back();
		
		temp->DestroyAll();
		
		delete temp;
	}
}

