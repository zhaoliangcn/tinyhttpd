#include "cmywindow.h" // class's header file
#include "debug.h"



// class constructor
CMyWindow::CMyWindow()
{

}

// class destructor
CMyWindow::~CMyWindow()
{

}
BOOL CMyWindow::InitInstance(HINSTANCE thishInstance)
{
	/* zero out the struct and set the stuff we want to modify */
	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = (WNDPROC) myWndProc; /* This is where we will send messages to */
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	
	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszClassName = "WindowClass";
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION);

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClass","MY HTTP SERVER",WS_VISIBLE|WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, /* x */
		CW_USEDEFAULT, /* y */
		640, /* width */
		480, /* height */
		NULL,NULL,hInstance,NULL);

	if(hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}
	
	SetWindowLong(hwnd, GWLP_USERDATA, (LONG_PTR)this);

	usrctls.Init(hwnd,hInstance);
	usrctls.CreateUserControls();

}
LRESULT CMyWindow::myWndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	LRESULT rt;

	CMyWindow* pWnd = (CMyWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	
	if(pWnd) // 调用 CMyWindow 类定义事件处理函数
	{
		rt = pWnd->WndProc(hwnd,Message, wParam, lParam);
	}
	else // 调用缺省的事件处理函数
	{
		rt = ::DefWindowProc (hwnd, Message, wParam, lParam);
	}
	return rt;
}

/* This is where all the input to the window goes to */
LRESULT CMyWindow::WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	
	switch(Message) {
		case WM_ERASEBKGND:
			{
				PAINTSTRUCT ps;				
				HDC hdc = BeginPaint(hwnd,&ps);
					RECT rect;
					GetClientRect(hwnd,&rect);//获得客户区大小 
					HBRUSH hbr= CreateSolidBrush(RGB(151,255,255));//创建画刷 
					HBITMAP  hMemBitmap=CreateCompatibleBitmap(hdc,rect.right,rect.bottom);	//建立内存位图				
					HDC   hdcMem=CreateCompatibleDC(hdc);//建立内存dc缓冲 
					SelectObject(hdcMem,hMemBitmap);//内存dc缓冲选入位图 
					FillRect(hdcMem,&rect,hbr);//绘制背景		
					//DrawText(hdcMem,TEXT("This is a Redraw Window"),-1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER)	;	//写入文本 
					BitBlt(hdc,0,0,rect.right,rect.bottom,hdcMem,0,0,SRCCOPY);//合并dc 
					DeleteDC(hdc);
					DeleteDC(hdcMem);
				EndPaint(hwnd,&ps);
			}
			break;
		
		case WM_PAINT:
			{
				PAINTSTRUCT ps;				
				HDC hdc = BeginPaint(hwnd,&ps);
					RECT rect;
					GetClientRect(hwnd,&rect);//获得客户区大小 
					HBRUSH hbr= CreateSolidBrush(RGB(151,255,255));//创建画刷 
					HBITMAP  hMemBitmap=CreateCompatibleBitmap(hdc,rect.right,rect.bottom);	//建立内存位图				
					HDC   hdcMem=CreateCompatibleDC(hdc);//建立内存dc缓冲 
					SelectObject(hdcMem,hMemBitmap);//内存dc缓冲选入位图 
					//FillRect(hdcMem,&rect,hbr);//绘制背景		
					DrawText(hdcMem,TEXT("This is a Redraw Window"),-1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER)	;	//写入文本 
					BitBlt(hdc,0,0,rect.right,rect.bottom,hdcMem,0,0,SRCCOPY);//合并dc 
					DeleteDC(hdc);
					DeleteDC(hdcMem);
				EndPaint(hwnd,&ps);
			}
			break;
		case WM_COMMAND:
			{
				usrctls.MsgProc(hwnd, Message, wParam, lParam);				
				
				break;
			}
		case WM_INITDIALOG:
			{
				MessageBox(NULL,(""),(""),MB_OK);
				break;
			}
		/* trap the WM_CLOSE (clicking X) message, and actually tell the window to close */
		case WM_CLOSE: {
			
			TerminateBackGroundThread();
			DestroyWindow(hwnd);
			break;
		}
		
		/* Upon destruction, tell the main thread to stop */
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
		
		/* All other messages (a lot of them) are processed using default procedures */
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
	
}
WORD CMyWindow::Run()
{
		/*
		This is the heart of our program where all input is processed and 
		sent to WndProc. Note that GetMessage blocks code flow until it receives something, so
		this loop will not produre unreasonably CPU usage
	*/
	while(GetMessage(&Msg, NULL, 0, 0) > 0) { /* If no error is received... */
		TranslateMessage(&Msg); /* Translate keycodes to chars if present */
		DispatchMessage(&Msg); /* Send it to WndProc */
	}
	return Msg.wParam;
}
unsigned __stdcall CMyWindow::ThreadFunc(void * param)
{
		CMyWindow* pWnd = (CMyWindow*)param;
		pWnd->myhttp.SetServerRoot(pWnd->ServerRoot);
		pWnd->myhttp.SockLoop();
		return 0;
}
BOOL CMyWindow::StartBackGroundThread()
{
	hth1 = (HANDLE)_beginthreadex( NULL,         // security
                                   0,            // stack size
                                   ThreadFunc,
                                   (void *)this,           // arg list
                                   CREATE_SUSPENDED,  // so we can later call ResumeThread()
                                   &uiThread1ID );

    if ( hth1 == 0 )
    	_DbgPrint("Failed to create background thread 1\n");
    DWORD   dwExitCode;
    GetExitCodeThread( hth1, &dwExitCode );  // should be STILL_ACTIVE = 0x00000103 = 259
    _DbgPrint( "initial background thread 1 exit code %d",dwExitCode );    
    ResumeThread( hth1 );  
}
BOOL CMyWindow::TerminateBackGroundThread()
{
	if(hth1)
	{
	DWORD   dwExitCode;
	myhttp.CloseSock();	
	GetExitCodeThread( hth1, &dwExitCode );
	_DbgPrint( "background thread 1 exited with code %d" ,dwExitCode);
	CloseHandle( hth1 );
	hth1= NULL;
	}
	return TRUE;
}
void CMyWindow::SetServerRootPath(char * rootpath)
{
	strcpy(ServerRoot,rootpath);
}

