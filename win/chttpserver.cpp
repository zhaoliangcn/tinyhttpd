#include "chttpserver.h" // class's header file
#include <stdio.h>
// class constructor
ChttpServer::ChttpServer() {
	sockevent=CreateEvent(NULL,TRUE,FALSE,"sockevent");
	closesock=false;
	working=true;
	InitSock();
	InitMime();
}
void ChttpServer::InitMime() {
	MimeTypes["doc"]	= "application/msword";
	MimeTypes["bin"]	= "application/octet-stream";
	MimeTypes["dll"]	= "application/octet-stream";
	MimeTypes["exe"]	= "application/octet-stream";
	MimeTypes["pdf"]	= "application/pdf";
	MimeTypes["p7c"]	= "application/pkcs7-mime";
	MimeTypes["ai"]		= "application/postscript";
	MimeTypes["eps"]	= "application/postscript";
	MimeTypes["ps"]		= "application/postscript";
	MimeTypes["rtf"]	= "application/rtf";
	MimeTypes["fdf"]	= "application/vnd.fdf";
	MimeTypes["arj"]	= "application/x-arj";
	MimeTypes["gz"]		= "application/x-gzip";
	MimeTypes["class"]	= "application/x-java-class";
	MimeTypes["js"]		= "application/x-javascript";
	MimeTypes["lzh"]	= "application/x-lzh";
	MimeTypes["lnk"]	= "application/x-ms-shortcut";
	MimeTypes["tar"]	= "application/x-tar";
	MimeTypes["hlp"]	= "application/x-winhelp";
	MimeTypes["cert"]	= "application/x-x509-ca-cert";
	MimeTypes["zip"]	= "application/zip";
	MimeTypes["7z"]	    = "application/7z";
	MimeTypes["cab"]	= "application/x-compressed";
	MimeTypes["arj"]	= "application/x-compressed";
	MimeTypes["aif"]	= "audio/aiff";
	MimeTypes["aifc"]	= "audio/aiff";
	MimeTypes["aiff"]	= "audio/aiff";
	MimeTypes["au"]		= "audio/basic";
	MimeTypes["snd"]	= "audio/basic";
	MimeTypes["mid"]	= "audio/midi";
	MimeTypes["rmi"]	= "audio/midi";
	MimeTypes["mp3"]	= "audio/mpeg";
	MimeTypes["vox"]	= "audio/voxware";
	MimeTypes["wav"]	= "audio/wav";
	MimeTypes["ra"]		= "audio/x-pn-realaudio";
	MimeTypes["ram"]	= "audio/x-pn-realaudio";
	MimeTypes["bmp"]	= "image/bmp";
	MimeTypes["gif"]	= "image/gif";
	MimeTypes["jpeg"]	= "image/jpeg";
	MimeTypes["jpg"]	= "image/jpeg";
	MimeTypes["tif"]	= "image/tiff";
	MimeTypes["tiff"]	= "image/tiff";
	MimeTypes["xbm"]	= "image/xbm";
	MimeTypes["wrl"]	= "model/vrml";
	MimeTypes["htm"]	= "text/html";
	MimeTypes["html"]	= "text/html";
	MimeTypes["c"]		= "text/plain";
	MimeTypes["cpp"]	= "text/plain";
	MimeTypes["def"]	= "text/plain";
	MimeTypes["h"]		= "text/plain";
	MimeTypes["txt"]	= "text/plain";
	MimeTypes["rtx"]	= "text/richtext";
	MimeTypes["rtf"]	= "text/richtext";
	MimeTypes["java"]	= "text/x-java-source";
	MimeTypes["css"]	= "text/css";
	MimeTypes["mpeg"]	= "video/mpeg";
	MimeTypes["mpg"]	= "video/mpeg";
	MimeTypes["mpe"]	= "video/mpeg";
	MimeTypes["avi"]	= "video/msvideo";
	MimeTypes["mov"]	= "video/quicktime";
	MimeTypes["qt"]		= "video/quicktime";
	MimeTypes["shtml"]	= "wwwserver/html-ssi";
	MimeTypes["asa"]	= "wwwserver/isapi";
	MimeTypes["asp"]	= "wwwserver/isapi";
	MimeTypes["cfm"]	= "wwwserver/isapi";
	MimeTypes["dbm"]	= "wwwserver/isapi";
	MimeTypes["isa"]	= "wwwserver/isapi";
	MimeTypes["plx"]	= "wwwserver/isapi";
	MimeTypes["url"]	= "wwwserver/isapi";
	MimeTypes["cgi"]	= "wwwserver/isapi";
	MimeTypes["php"]	= "wwwserver/isapi";
	MimeTypes["wcgi"]	= "wwwserver/isapi";
	MimeTypes["js"]		=	"application/x-javascript";
}
// class destructor
ChttpServer::~ChttpServer() {
	CloseHandle(sockevent);
	TermWorkingThread();
	WSACleanup( );
}
BOOL ChttpServer::InitSock() {
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested =MAKEWORD( 2, 2 );
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		return FALSE;
	}
	if ( LOBYTE( wsaData.wVersion ) != 2 ||
	        HIBYTE( wsaData.wVersion ) != 2 ) {
		WSACleanup( );
		return FALSE;
	}
	mysock=socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in my_addr;
	my_addr.sin_family=AF_INET;
	my_addr.sin_port=htons(80);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	memset(&(my_addr.sin_zero),0,8);
	if (bind(mysock, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) {
		perror("bind error");
		return FALSE;
	}
	return TRUE;
}
BOOL ChttpServer::SockLoop() {
	StartWorkingThread();
	listen(mysock,10);

	SOCKADDR_IN sin;
	int addlen=sizeof(sin);

	int lazytime=GetCurrentTime();
	while(!closesock) {
		if((GetCurrentTime()-lazytime)>LAZY_TIME) {
			lazytime=GetCurrentTime();
			mymemcache.FlushCache();
		}
		PHTTPREQUEST req=requestqueue.NewRequest();
		req->clientsocket =accept(mysock,(struct sockaddr*)&sin,&addlen);
		if(req->clientsocket==-1) {
			Sleep(1);
			requestqueue.DeleteRequest(req);
			continue;
		} else {
			int recvlen=0;
			if((recvlen=recv(req->clientsocket ,req->request,sizeof(req->request),0))>0) {
				req->requestlen=recvlen;
				requestqueue.AddRequest(req);
				SetEvent(sockevent);
			} else {
				closesocket(req->clientsocket );
				requestqueue.DeleteRequest(req);
			}
		}
	}
	return FALSE;
}
void ChttpServer::CloseSock() {
	closesock =true;
	shutdown(mysock,SD_BOTH);
	closesocket(mysock);
}
BOOL ChttpServer::ParseURI(PHTTPREQUEST req,char * URI,int& urisize)
{
	char * space = strstr(req->request," ");
	char * sep = strrstr(req->request," HTTP/");
	if(space && sep)
	{
		int size = sep-space-1;
		if(urisize>=size)
		{
			strncpy(URI,space+1,size);
			urisize=size;	
			return TRUE;
		}				
	} 
	return FALSE;
}
HTTPHEADER * ChttpServer::FindHeader(PHTTPREQUEST req,const char * headername)
{
	HTTPHEADER * header=req->headers;
	while(header)
	{
		if(stricmp(header->headername,headername)==0)
		{
			break;
		}
		else
		{
			header=header->NextHeader;
		}
	} 
	return header;
}
BOOL ChttpServer::ParseHeaders(PHTTPREQUEST req)
{
	char * headerpos = strstr(req->request,backreturn);
	if(headerpos)
	{
		headerpos+=2;
	}
	char * headerend =  strstr(req->request,doublebackreturn);
	if(headerpos && headerend)
	{
		
		HTTPHEADER * header =new HTTPHEADER; 
		req->headers = header; 
		char * phead = headerpos;
		while(phead<headerend)
		{
			char * sep = strstr(phead,":");
			if(sep)
			{
				sep+=1;
			} 
			if(sep)
			{
				strncpy(header->headername,phead,sep-phead);
				char * tempsep=strstr(sep,backreturn) ;
				if(tempsep)
				strncpy(header->headercontent,sep+1,tempsep-sep-1); 
				_DbgPrint("HEADERNAME %s",header->headername);
				_DbgPrint("HEADERDATA %s",header->headercontent);
			}
			phead=strstr(phead,backreturn);
			if(phead)
			{
				phead+=2;
				header->NextHeader =new HTTPHEADER; 
				header=header->NextHeader ;
			}
			else
			{
				break;
			}
			
		}
	}
	if(strcmp(&req->request[req->requestlen-4],doublebackreturn)==0)
	{
		_DbgPrint("no body ");
	}
	return FALSE;
}
void ChttpServer::ParseRequest() {
	PHTTPREQUEST req;
	req=requestqueue.GetRequest();
	char responsebuf[2048];
	memset(responsebuf,0,sizeof(responsebuf));
	if(req->request[0]=='G'&&req->request[1]=='E'&&req->request[2]=='T') {
		req->requesttype = REQUEST_GET;
		ParseHeaders(req);
		ProcessGet(req);
	} else if(req->request[0]=='P'&&req->request[1]=='O'&&req->request[2]=='S'&&req->request[3]=='T') {
		req->requesttype = REQUEST_POST;
		ParseHeaders(req);
		ProcessPost(req);
	} else {
		sprintf(responsebuf,"HTTP/1.1 400 Invalid Request \r\n\r\n");
		if(send(req->clientsocket ,responsebuf,strlen(responsebuf),0)==-1) {
			_DbgPrint("send response error");
		}
		shutdown(req->clientsocket ,SD_BOTH);
		closesocket(req->clientsocket );
		requestqueue.DeleteRequest(req);
		return ;
	}
	requestqueue.DeleteRequest(req);
	return ;
}
void ChttpServer::GetFileMimeType(char * filename,std::string &mimetype) {
	char fileext[MAX_PATH];
	memset(fileext,0,sizeof(fileext));
	char * ext=strrchr(filename,'.');
	if(ext) {
		memcpy(fileext,ext+1,strlen(ext)-1);
	}
	_DbgPrint("%s",fileext);
	mimetype="text/plain";
	MIMETYPES::iterator it;
	it = MimeTypes.find(fileext);
	if(it != MimeTypes.end())
		mimetype = (*it).second;
	mimetype+=";charset=UTF8";
	_DbgPrint("%s",mimetype.c_str());
}
BOOL ChttpServer::ProcessGet(PHTTPREQUEST req) {
	char URI[512];
	int urisize =512;
	ParseURI(req,URI,urisize);
	char responsebuf[2048];
	memset(responsebuf,0,sizeof(responsebuf));
	//sprintf(responsebuf,"HTTP/1.1 200 OK \r\nContent-Type: text/plain\r\nContent-Length: 54\r\n\r\n this is simple webpage!");
	char filename[MAX_PATH];
	memset(filename,0,sizeof(filename));
	if(URI[0]=='/' && strlen(URI)>1)
	{		
		sprintf(filename,"%s%s",getserverroot(),&URI[1]);
		_DbgPrint("%s",filename);
	}
	else
	{
		sprintf(filename,"%s%s",getserverroot(),"index.html");
	}
	_DbgPrint("%s",req->request);
	std::string szContentType;
	GetFileMimeType(filename,szContentType);

	PMEMCACHESECTION pmemfile=mymemcache.FindFile(filename);

	if(pmemfile==NULL) {
		pmemfile=mymemcache.LoadFile(filename);
	}
	if(pmemfile!=NULL) {
		if(req->request[0]=='G'&&req->request[1]=='E'&&req->request[2]=='T') {
			sprintf(responsebuf,"HTTP/1.1 200 OK \r\nContent-Type: %s\r\nContent-Length: %d\r\n\r\n",szContentType.c_str(),pmemfile->totallen);
			if(send(req->clientsocket ,responsebuf,strlen(responsebuf),0)==-1) {
				_DbgPrint("send response error");
			}
			do {
				if(send(req->clientsocket ,pmemfile->cache,pmemfile->reallen,0)==-1) {
					_DbgPrint("send response error");
				}
				pmemfile=pmemfile->nextsection;
			} while(pmemfile!=NULL);

		}
	} else {
		sprintf(responsebuf,"HTTP/1.1 404 Not Found \r\n\r\n");
		if(send(req->clientsocket ,responsebuf,strlen(responsebuf),0)==-1) {
			_DbgPrint("send response error");
		}
	}
	closesocket(req->clientsocket );
	return TRUE;
}
BOOL ChttpServer::ProcessPost(PHTTPREQUEST req) {
	HTTPHEADER * header_clength = FindHeader(req,HTTP_HEADER_CLENGTH);
	if(header_clength)
	{
		_DbgPrint("%s %s ",HTTP_HEADER_CLENGTH,header_clength->headercontent);
	}
	return FALSE;
}
unsigned __stdcall ChttpServer::SockWorkingThreadFunc(void * param) {
	ChttpServer * pSock=(ChttpServer *)param;
	while(true) {
		WaitForSingleObject(pSock->sockevent,INFINITE);
		if(!pSock->working)
			break;
		while(pSock->requestqueue.GetSize()>0) {
			_DbgPrint(("queuesizeis%d"),pSock->requestqueue.GetSize());
			pSock->ParseRequest();
		}
		ResetEvent(pSock->sockevent);
	}
	return 0;
}
BOOL ChttpServer::StartWorkingThread() {
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
	_DbgPrint( "initial thread 2 exit code %d",dwExitCode );
	ResumeThread( hth1 );
	return TRUE;
}
BOOL ChttpServer::TermWorkingThread() {
	DWORD   dwExitCode;
	SetEvent(sockevent);
	working=false;
	GetExitCodeThread( hth1, &dwExitCode );
	_DbgPrint( "thread 2 exited with code %d" ,dwExitCode);
	CloseHandle( hth1 );
	return TRUE;
}

void ChttpServer::SetServerRoot(string root) {
	httpserverroot=root;
}
const char * ChttpServer::getserverroot() {
	return httpserverroot.c_str();
}
