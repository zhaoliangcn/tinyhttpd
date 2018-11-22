#include <iostream>
#include "cmysocket.h"
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char *argv[]) {
	
	CMySocket mysocket;
	mysocket.StartWorkingThread();
	while(true)
	{
		Sleep(1);
	}
	return 0;
}
