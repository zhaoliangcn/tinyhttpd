#ifndef CSAFEREQUESTQUEUESET_H
#define CSAFEREQUESTQUEUESET_H
#include <vector>
#include "chttprequestqueue.h"
using namespace std;
/*
 * No description
 */
class CSafeRequestQueueSet
{
	friend class CHttpRequestQueue;
	// private section
	public:
		// class constructor
		CSafeRequestQueueSet();
		// class destructor
		~CSafeRequestQueueSet();
		
		CHttpRequestQueue * MakeNewQueue();
		
		void DestroyAll();
		
		vector<CHttpRequestQueue*> queueset;
		
	protected:
};

#endif // CSAFEREQUESTQUEUESET_H

