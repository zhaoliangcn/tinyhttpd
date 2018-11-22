#include "cmemeorycache.h" // class's header file

// class constructor
CMemeoryCache::CMemeoryCache() {
}

// class destructor
CMemeoryCache::~CMemeoryCache() {
	DestroyAll();
}
PMEMCACHESECTION CMemeoryCache::GetNewOne() {
	PMEMCACHESECTION pcache=new MEMCACHESECTION;
	if(pcache!=NULL)
		memcache.push_back(pcache);
	return pcache;
}
PMEMCACHESECTION CMemeoryCache::FindFile(char * filename) {
	for(MyMemCache::iterator i=memcache.begin(); i!=memcache.end();) {
		PMEMCACHESECTION ptemp=*i;
		if(strcmp(filename,ptemp->FileName)==0) {
			UpdateLastUseTime(ptemp);
			return ptemp;
		} else {
			if((GetCurrentTime()-GetLastUseTime(ptemp))>LAZY_TIME) {
				EraseOne(ptemp);
				i=memcache.erase(i);
			} else {
				i++;
			}
		}

	}
	return NULL;
}
PMEMCACHESECTION CMemeoryCache::LoadFile(char * FileName) {
	HANDLE handle=CreateFile(FileName,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(INVALID_HANDLE_VALUE==handle)
		return NULL;
	DWORD sizelow=0,sizehigh=0;
	sizelow=::GetFileSize(handle,&sizehigh);
	DWORD toread=0;
	if(sizelow>CACHE_SECTION_SIZE)
		toread=CACHE_SECTION_SIZE;
	else
		toread=sizelow;
	DWORD readcount=0;
	PMEMCACHESECTION pcache=GetNewOne();
	if(pcache==NULL)
		return NULL;
	if(!ReadFile(handle,pcache->cache,toread,&readcount,NULL)) {
		delete pcache;
		return NULL;
	}

	pcache->totallen=sizelow;
	pcache->pos=0;
	pcache->reallen=readcount;
	memcpy(pcache->FileName,FileName,strlen(FileName));
	sizelow-=readcount;
	while(sizelow>0) {
		PMEMCACHESECTION ptemp=new MEMCACHESECTION;
		PMEMCACHESECTION ptail;
		if(pcache->nextsection==NULL) {
			ptail=pcache;
			pcache->nextsection=ptemp;
		} else {
			ptail=pcache->nextsection;
			while(ptail->nextsection!=NULL) {
				ptail=ptail->nextsection;
			}
			ptail->nextsection=ptemp;
		}
		if(sizelow>CACHE_SECTION_SIZE)
			toread=CACHE_SECTION_SIZE;
		else
			toread=sizelow;
		ptemp->pos=ptail->pos+readcount;
		if(!ReadFile(handle,ptemp->cache,toread,&readcount,NULL)) {
			ptail->nextsection=NULL;
			delete ptemp;
			break;
		}
		ptemp->reallen=readcount;
		sizelow-=readcount;
	}
	CloseHandle(handle);
	UpdateLastUseTime(pcache);



	return pcache;
}
void CMemeoryCache::UpdateLastUseTime(PMEMCACHESECTION memsection) {
	if(memsection==NULL)
		return ;
	memsection->lastusetime=GetCurrentTime();
	return ;
}
unsigned int CMemeoryCache::GetLastUseTime(PMEMCACHESECTION memsection) {
	if(memsection==NULL)
		return 0;
	return memsection->lastusetime;
}
void CMemeoryCache::EraseOne(PMEMCACHESECTION memsection) {
	if(memsection==NULL)
		return ;
	PMEMCACHESECTION temp=memsection->nextsection;

	while(temp!=NULL) {
		PMEMCACHESECTION temp1=temp;
		temp=temp->nextsection;
		delete temp1;

	}
	delete memsection;
	return ;
}
void CMemeoryCache::DestroyAll() {
	while(memcache.size()>0) {
		PMEMCACHESECTION pcache=memcache.back();
		memcache.pop_back();
		PMEMCACHESECTION temp=pcache->nextsection;

		while(temp!=NULL) {
			PMEMCACHESECTION temp1=temp;
			temp=temp->nextsection;
			delete temp1;
		}
		delete pcache;
	}

}
void CMemeoryCache::FlushCache() {
	if(GetCacheMemoryCount()>MAX_CACHE_SIZE) {
		for(MyMemCache::iterator i=memcache.begin(); i!=memcache.end();) {
			PMEMCACHESECTION ptemp=*i;
			if((GetCurrentTime()-GetLastUseTime(ptemp))>LAZY_TIME) {
				EraseOne(ptemp);
				i=memcache.erase(i);
			} else {
				i++;
			}
		}
	}
	return ;
}
int CMemeoryCache::GetCacheMemoryCount() {
	int count=0;
	for(MyMemCache::iterator i=memcache.begin(); i!=memcache.end(); i++) {
		PMEMCACHESECTION ptemp=*i;
		count+=ptemp->totallen;
	}
	return count;
}
