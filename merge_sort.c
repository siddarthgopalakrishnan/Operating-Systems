// Merge sort using threads
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

typedef struct pair_int_int {
	int l;
	int r;
} pii; // pair struct to store l and r values for each thread

int N; //number of elements
int *arr = NULL, *temp = NULL;

void *mergesort(void *args) {
	pii *p = (pii*)args;
	int l = p->l, r = p->r;
	if(r<=l) return NULL;
	int mid = l+(r-l)/2;
	pthread_t t1, t2; // create two threads for subproblems
	int rc;
	pii p1, p2;
	p1.l = l; p1.r = mid; // bounds for first subproblem
	p2.l = mid+1; p2.r = r; // bounds for second subproblem

	rc = pthread_create(&t1, NULL, mergesort, (void*)&p1); // left subproblem
	if(rc) {
		printf("Error creating left subproblem, rc value = %d\n", rc);
		exit(1);
	}
	rc = pthread_create(&t2, NULL, mergesort, (void*)&p2); // right subproblem
	if(rc) {
		printf("Error creating right subproblem, rc value = %d\n", rc);
		exit(1);
	}
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	// Merging left and right subproblems
	int l1=l, l2=mid+1, i;
	for(i=l; l1<=mid && l2<=r; i++) {
		temp[i] = (arr[l1]<=arr[l2]) ? arr[l1++] : arr[l2++];
	}
	while(l1<=mid) temp[i++] = arr[l1++]; // remaining elements in left part, if any
	while(l2<=r) temp[i++] = arr[l2++]; // remaining elements in right part, if any
	for(i=l;i<=r;i++) arr[i] = temp[i]; // put everything back in arr
	pthread_exit(NULL);
}

int main(void) {
	int N, rc;
	char dummy;
	pthread_t parent;
	scanf("%d", &N);
	arr = (int*)malloc(N*sizeof(int)); // for having integers
	temp = (int*)malloc(N*sizeof(int)); // temp array for merging
	for(int i = 0; i < N; i++) {
		scanf("%d", &arr[i]);
		dummy = getchar();
	}
	pii p;
	p.l = 0, p.r = N-1;
	rc = pthread_create(&parent, NULL, mergesort, (void*)&p);
	if(rc) {
		printf("Error in creating thread, rc value = %d\n", rc);
		exit(1);
	}
	pthread_join(parent, NULL);
	for(int i=0; i<N; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
	pthread_exit(NULL);
	return 0;
}