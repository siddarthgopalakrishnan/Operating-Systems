// https://www.codechef.com/OSP32020/problems/GRPTHRD
// Grep Threading
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#define max_len 100000

typedef struct bucket {
	char struct_str[max_len];
	char struct_check[max_len];
} bucket;

void* func(void * container) {
	bucket* strings = (bucket *)container;
	char check[max_len];
	char str[max_len];
	strcpy(check, strings->struct_check);
	strcpy(str, strings->struct_str);
	if(strcasestr(str, check) != NULL) {
		puts(str);
	}
	pthread_exit(NULL);
}

int main() {
	int n, t;
	scanf("%d ", &n);
	char *arr[n];
	for(int i=0; i<n; i++) {
		arr[i] = malloc(max_len * sizeof(char));
		char temp[max_len];
		scanf("%[^\n]%*c", temp);
		strcpy(arr[i], temp);
	}
	scanf("%d", &t);
	while(t--) {
		bucket container;
		char check[max_len];
		scanf("%s", check);
		strcpy(container.struct_check, check);
		for(int i=0; i<n; i++) {
			char str[max_len];
			strcpy(str, arr[i]);
			strcpy(container.struct_str, str);
			int rc;
			pthread_t t1;
			rc = pthread_create(&t1, NULL, *func, (void *)&container);
			if(rc) {
				printf("Error creating thread; rc value = %d\n", rc);
				exit(1);
			}
			pthread_join(t1, NULL);
		}
	}
	return 0;
}