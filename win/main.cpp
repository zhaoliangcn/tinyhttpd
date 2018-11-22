#include <winsock2.h>
#include <windows.h>
#include "resource.h"
#include "cmywindow.h"

CMyWindow mywin;
CMyWindow * mainwnd;
/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	mainwnd=&mywin;
	mywin.InitInstance(hInstance);	
	mywin.Run();

}
