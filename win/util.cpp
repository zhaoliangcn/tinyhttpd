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
char *strrstr(const char *dst, const char *src)
{
	const char *pdst = dst;
	const char *psrc = src;
	char *right= NULL;
	while (*dst)
	{
		while (*pdst == *psrc)
		{
			if (*pdst== '\0')                     
				return right=(char *)dst;
			else
			{
				pdst++;
				psrc++;
			}
		}
		if (*psrc == '\0') 
			right = (char *)dst;
		pdst = ++dst;
		psrc = src;
	}
	return right;
}
