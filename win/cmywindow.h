#ifndef CMYWINDOW_H
#define CMYWINDOW_H
#include <winsock2.h>
#include <windows.h>
#include "resource.h"
#include "cusercontrols.h"
#include <stdio.h>
#include <process.h>
#include <winbase.h>
 
#include "chttpserver.h"



/*
 * No description
 */
class CMyWindow
{
	// private section
	public:
		// class constructor
		CMyWindow();
		// class destructor
		~CMyWindow();
		BOOL InitInstance(HINSTANCE thishInstance);		
		WORD Run();		
		const HWND GetWnd()
		{
			return hwnd;
		}
		const CMyWindow * getWindow()
		{
			return this;
		}
		BOOL StartBackGroundThread();
		BOOL TerminateBackGroundThread();
		void SetServerRootPath(char * rootpath);			
	protected:		
	 	LRESULT WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
	 	static LRESULT myWndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);	
		WNDCLASSEX wc; /* A properties struct of our window */
		HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
		MSG Msg; /* A temporary location for all messages */		
		HINSTANCE hInstance;
	private:
		char ServerRoot[512];
		CUserControls usrctls;
		static unsigned __stdcall ThreadFunc(void * param);	
		HANDLE   hth1;
    	unsigned  uiThread1ID;
    	 ChttpServer myhttp;

};

#endif // CMYWINDOW_H

