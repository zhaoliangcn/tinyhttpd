#ifndef CMEMEORYCACHE_H
#define CMEMEORYCACHE_H

#define CACHE_SECTION_SIZE (32768)
#define MAX_CACHE_SIZE (1024*1024*20) 
#define LAZY_TIME	(50000)
#include<windows.h>
#include<vector>
#include<list>
using namespace std;
typedef struct memcachesection
{
	memcachesection()
	{
		memset(FileName,0,sizeof(FileName));
		memset(cache,0,sizeof(cache));
		nextsection=NULL;
		prevsection=NULL;
		pos=0;
		reallen=0;
		totallen=0;
		lastusetime=0;
		visitedcount=0;
	}
	~memcachesection(){
	}	
	int pos;
	int reallen;
	int totallen;
	unsigned int lastusetime;
	unsigned int visitedcount;
	memcachesection * prevsection ;
	memcachesection * nextsection ;
	char FileName[MAX_PATH];		
	char cache[CACHE_SECTION_SIZE];		
}MEMCACHESECTION,*PMEMCACHESECTION;

typedef list<PMEMCACHESECTION> MyMemCache;
/*
 * No description
 */
class CMemeoryCache
{
	// private section
	public:
		// class constructor
		CMemeoryCache();
		// class destructor
		~CMemeoryCache();
		
		PMEMCACHESECTION GetNewOne();
		void DestroyAll();
		PMEMCACHESECTION FindFile(char * filename);
		PMEMCACHESECTION LoadFile(char * FileName);
		void UpdateLastUseTime(PMEMCACHESECTION memsection);
		unsigned int GetLastUseTime(PMEMCACHESECTION memsection);
		void EraseOne(PMEMCACHESECTION memsection);
		void FlushCache();
		int GetCacheMemoryCount();
		
	protected:
		MyMemCache memcache;
};

#endif // CMEMEORYCACHE_H

