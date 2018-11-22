#include "debug.h"
void _DbgPrint(const char * sz, ...)
{
	char szData[512]={0};	
	va_list args;
	va_start(args, sz);
	_vsnprintf(szData, sizeof(szData) - 1, sz, args);
	va_end(args);	
	OutputDebugString(szData);
}
