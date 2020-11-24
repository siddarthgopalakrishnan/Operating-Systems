// Shared Memory Game
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

int mycomparator (const void *x, const void *y) {	// comparator to sort
	return (*(int*)x - *(int*)y);
}

int binsearch(int arr[], int low, int high, int ele) { // binary search to find number
	int l=low, r=high;
	while(l <= r) {
		int mid = (r+l)/2;
		if(arr[mid] > ele) r=mid-1;
		else if(arr[mid] < ele) l=mid+1;
		else return mid;
	}
	return -1;
}

int main() {
	int shmid_a, shmid_p;
	int *shmptr_ad; // storing admin number
	int *iterations; // storing number of iterations for each player
	char dummy;
	int N, player_val = 0; // number of players
	scanf("%d", &N); dummy = getchar();
	int *p_nums_size = (int *)malloc(sizeof(int) * N); // stores number of numbers for ith person
	int **p_nums = (int **)malloc(sizeof(int *) * N); // stores numbers of the ith person

	// inputting players' numbers
	for(int i=0; i<N; i++) {
		int size; scanf("%d", &size);
		dummy = getchar();
		p_nums_size[i] = size;
		p_nums[i] = (int *)malloc(sizeof(int) * size);
		for(int num=0; num<size; num++) {
			scanf("%d", &p_nums[i][num]);
			dummy = getchar();
		}
		qsort(p_nums[i], size, sizeof(int), mycomparator);
	}

	// inputting admin's numbers
	int N_admin;
	scanf("%d", &N_admin); dummy = getchar();
	if((shmid_a = shmget(1370, sizeof(int) * N_admin, 0666 | IPC_CREAT)) == -1) {
		perror("Error in shmget\n");
		exit(1);
	}
	shmptr_ad = shmat(shmid_a, 0, 0);
	if(shmptr_ad == (int *)-1) {
		exit(2);
	}
	for(int num=0; num<N_admin; num++) {
		scanf("%d", &shmptr_ad[num]);
		dummy = getchar();
	}

	if((shmid_p = shmget(1371, N * sizeof(int), 0666 | IPC_CREAT)) == -1) {
		perror("Error in shmget\n");
		exit(1);
	}
	iterations = shmat(shmid_p, 0, 0);
	if(iterations == (int *)-1) {
		exit(2);
	}

	for(int i = 0; i < N; i++) {
		if(fork() == 0) { // creating child processes
			int ind;
			int iters = 0, count = 0;
			for(int num = 0; num < N_admin; num++) {
				if(p_nums[i][0] == 100000000) break;
				iters++;
				ind = binsearch(p_nums[i], 0, p_nums_size[i], shmptr_ad[num]);
				if(ind != -1) {
					++count;
					int temp;
					for(temp = ind; temp < p_nums_size[i]-1; temp++) {
						p_nums[i][temp] = p_nums[i][temp+1];
					}
					p_nums[temp] = 100000000;
				}
				else break;
				if(count == p_nums_size[i])
					break;
			}
			iterations[i] = (count == p_nums_size[i]) ? iters : 100000000;
			exit(0);
		}
		else wait(NULL);
	}

	for(int i=0; i<N; i++) wait(NULL);
	for(int i=1; i<N; i++) {
		if(iterations[player_val] > iterations[i]) {
			player_val = i;
		}
	}
	player_val += 1;
	printf("\nplayer %d\n", player_val);
	return 0;
}