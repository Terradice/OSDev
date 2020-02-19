#include <libc/stdio.h>
#include <video/vga.h>
#include <stdint.h>
#include <stdarg.h>

void itoa (char *buf, int base, int d) {
	char *p = buf;
	char *p1, *p2;
	unsigned long ud = d;
	int divisor = 10;
  
  /* If %d is specified and D is minus, put ‘-’ in the head. */
	if (base == 'd' && d < 0) {
		*p++ = '-';
		buf++;
		ud = -d;
	}
	else if (base == 'x')
	divisor = 16;

  /* Divide UD by DIVISOR until UD == 0. */
	do
	{
		int remainder = ud % divisor;

		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
	}
	while (ud /= divisor);

  /* Terminate BUF. */
	
  
  /* Reverse BUF. */
	p1 = buf;
	p2 = p - 1;
	while (p1 < p2){
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}
}

void vprintf(void (*write)(char), const char* format, va_list arg) {
	char c;
	while((c = *format++) != 0) {
		if(c == '%') {
			c = *format++;
			switch(c) {
				case 's': {
					char* str = va_arg(arg, char*);
					char cur;
					while((cur = *str++) != 0) {
						write(cur);
					}
					break;
				}

				case 'i': {
	                int num = va_arg(arg, int);
	                int k = 0;
	                char buf[32];
	                do {
	                    buf[k] = (num % 10) + '0';
	                    num /= 10;
	                    k++;
	                } while (num);


                	for (int i = k - 1; i >= 0; i--)
                    	write(buf[i]);
					break;
				}

				case 'x': {
					const char digits[17] = "0123456789ABCDEF";
					unsigned int num = va_arg(arg, unsigned int);
					int k = 0;
					char buf[32];
	                do {
	                	buf[k] = digits[num % 16];
	                    num /= 16;
	                    k++;
                	} while (num);
                	for (int i = k - 1; i >= 0; i--)
                    	write(buf[i]);
				}
			}
		} else {
			write(c);
		}
	}
}

void printf(void (*write)(char), const char* format, ...) {
	va_list arg;
	va_start(arg, format);
	vprintf(write, format, arg);
	va_end(arg);
}