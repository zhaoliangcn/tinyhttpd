//util.cpp
#include <string.h>
int substrpos(const char * string ,const char sub,int startpos) {
	if(startpos>=strlen(string))
		return -1;
	int i=startpos;
	while(string[i]!='\0'&&i<strlen(string)) {
		if(string[i]==sub)
			return i;
		i++;
	}
	return -1;
}
