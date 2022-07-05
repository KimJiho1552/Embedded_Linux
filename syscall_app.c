#include <autoconf.h>
#include <stdio.h>
#include <unistd.h>
#include <asm/unistd.h>

int main() {
	long l;
	int dir = 1;
	char ox[7] = {'a', ':', 'a', ':', 'a', ':', 'a'};
	
	puts("X:X:X:X");
	l = 0x01;
	while(1) {
		int i = 0;
		l = syscall(__NR_mysyscall, l);
		for(i = 0; i < 4; i++) {
			if((l & (0x01 << i)) == (0x01 << i))
				ox[i*2] = 'O';
			else
				ox[i*2] = 'X';
		}
		puts(ox);
		usleep(100000);
		if(l == 0x00) {
			if(dir == 1) {
				l = 0x08;
				dir = 0;
			}
			else {
				l = 0x01;
				dir = 1;
			}
		}
		else
			if(dir == 1)
				l = l << 1;
			else
				l = l >> 1;
	}
	if(l < 0) {
		perror("syscall");
		return 1;
	}
	return 0;
}
