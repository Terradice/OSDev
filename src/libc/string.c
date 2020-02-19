#include <libc/string.h>
#include <stddef.h>

size_t strlen(const char* str) {
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

void strcat(char* dest, char* src, int pos) {
	int olen = pos || strlen(dest);
	int nlen = strlen(src);

	for(int i = olen; i < olen+nlen; i++) {
		dest[i] = src[i-olen];
	}
}

int strcmp(const char *s1, const char *s2) {
    while (*s1 == *s2)
    {
        if (*s1 == '\0')
        {
            return 0;
        }

        ++s1;
        ++s2;
    }

    return *s1 - *s2;
}