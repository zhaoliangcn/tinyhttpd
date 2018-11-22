#ifndef CUSERCONTROLS_H
#define CUSERCONTROLS_H
#include <winsock2.h>
#include <windows.h>
#include "resource.h"

/*
 * No description
 */
class CUserControls
{
	// private section
	public:
		// class constructor
		CUserControls();
		// class destructor
		~CUserControls();
		
		BOOL CreateUserControls();
		BOOL Init(HWND hwnd,HINSTANCE hInstance);
		void MsgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
	protected:
		
		HWND hwndParent;
		HINSTANCE hInstanceParent;
	private:
		HWND hbutton;
		HWND hbutton2;
		HWND hbutton3;
		HWND hedit1;
};

#endif // CUSERCONTROLS_H

