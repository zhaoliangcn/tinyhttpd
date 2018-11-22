#include "cusercontrols.h" // class's header file
#include "cmywindow.h"
extern CMyWindow * mainwnd;
// class constructor
CUserControls::CUserControls()
{
	// insert your code here
}

// class destructor
CUserControls::~CUserControls()
{
	// insert your code here
}
BOOL CUserControls::CreateUserControls()
{
	hbutton=CreateWindow(	"button",
							"Start",
							WS_CHILD|WS_VISIBLE,
							80,              // initial x position
                          	0,              // initial y position
                          	80,              // initial x size
                          	30,              // initial y size
                          	hwndParent,                       // parent window handle
                          	(HMENU)ID_BUTTON1,                       // window menu handle
                          	hInstanceParent,                  // program instance handle
                          	NULL) ;                     // creation parameters
	hbutton2=CreateWindow(	"button",
							"Stop",
							WS_CHILD|WS_VISIBLE,
							80,              // initial x position
                          	40,              // initial y position
                          	80,              // initial x size
                          	30,              // initial y size
                          	hwndParent,                       // parent window handle
                          	(HMENU)ID_BUTTON2,                       // window menu handle
                          	hInstanceParent,                  // program instance handle
                          	NULL) ; 
                          	
    hbutton3=CreateWindow(	"button",
							"Restart",
							WS_CHILD|~WS_VISIBLE,
							160,              // initial x position
                          	40,              // initial y position
                          	80,              // initial x size
                          	30,              // initial y size
                          	hwndParent,                       // parent window handle
                          	(HMENU)ID_BUTTON3,                       // window menu handle
                          	hInstanceParent,                  // program instance handle
                          	NULL) ; 
	hedit1=CreateWindow(	"edit",
							"F:\\share\\WEB\\appjs\\",
							WS_CHILD|WS_VISIBLE,
							80,              // initial x position
                          	80,              // initial y position
                          	400,              // initial x size
                          	20,              // initial y size
                          	hwndParent,                       // parent window handle
                          	(HMENU)ID_EDIT1,                       // window menu handle
                          	hInstanceParent,                  // program instance handle
                          	NULL) ; 
	return TRUE;
}
BOOL CUserControls::Init(HWND hwnd,HINSTANCE hInstance)
{
	hwndParent=hwnd;
	hInstance=hInstanceParent;
	return TRUE;	
}
void CUserControls::MsgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	char path[512];
	
	switch(HIWORD(wParam))
	{
		case BN_CLICKED:
		{
			switch(LOWORD(wParam))
			{
				case ID_BUTTON1:
				{					
					GetWindowTextA(hedit1,path,512);
					mainwnd->SetServerRootPath(path);
					mainwnd->StartBackGroundThread();
					EnableWindow(hbutton,FALSE);
						break;
				}
				case ID_BUTTON2:
				{
					SendMessage(mainwnd->GetWnd(),WM_CLOSE,0,0);
					break;
				}	
				case ID_BUTTON3:
				{
					mainwnd->TerminateBackGroundThread();
					GetWindowTextA(hedit1,path,512);
					mainwnd->SetServerRootPath(path);
					mainwnd->StartBackGroundThread();
					EnableWindow(hbutton,FALSE);					
				}												
			}
		}
		break;		
	}
	
}
