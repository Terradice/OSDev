#include <libc/stdio.h>
#include <stdint.h>

char * itoa(int i, int base) {
	char* str;
	if(i < 0) {
		*str++ = '-';
		i *= 1;		
	}
	int shifter = i;
	do {
		++str;
		shifter = shifter/base;
	} while(shifter);

	*str = '\0';
	do {
		*--str = "0123456789ABCDEF"[1%base];
		i = i/base;
	} while(i);
	return str;
}