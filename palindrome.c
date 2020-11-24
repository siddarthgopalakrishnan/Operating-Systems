// https://www.codechef.com/OSPA2020/problems/PFORK
// Palindrome with fork
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
	char s[100001];
	scanf("%s", s);
	int len = strlen(s);
	int p;
	p = fork();
	if(p < 0) {
		return 0;
	}
	if(p == 0) {
		//printf("From child : %s\n", s);
		int i = 0, j = len-1;
		while(i<=j) {
			if(s[i++] != s[j--]) {
				exit(0);
			}
		}
		exit(1);
	}
	else {
		int flag = 0;
		wait(&flag);
		if(flag/255) printf("yes\n");
		else printf("no\n");
	}
	return 0;
}
