// https://www.codechef.com/OSP32020/problems/MLFQ
// Multi-level Feedback Queue
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define ll long long

typedef struct process_struct {
	int arrivalT, cpuT, pri; // arrival time, burst time, priority
	int remainingT, runT, p_id;
} proc;

ll comp_times[100]; // to store the completion times
proc q1[100], q2[100], q3[100], q4[100]; // the 4 queues

int mycomp1(const void *a, const void *b) {
	proc *x = (proc *)a;
	proc *y = (proc *)b;
	return (x->arrivalT != y->arrivalT) ? (x->arrivalT - y->arrivalT) : (x->pri - y->pri);
}

int srft_comparator(const void *a, const void *b) {
	proc *x = (proc *)a;
	proc *y = (proc *)b;
	return (x->remainingT != y->remainingT) ? (x->remainingT - y->remainingT) : (x->pri - y->pri);
}

int main() {
	ll clock = 0;
	int quant1 = 2, quant2 = 3, quant3 = 5; // time quantums
	int n, completed = 0; // number of processes
	scanf("%d", &n);
	for(int i=0; i<n; i++) {
		scanf("%d %d %d", &q1[i].arrivalT, &q1[i].cpuT, &q1[i].pri);
		q1[i].p_id = i + 1; // to address the processes
		q1[i].runT = 0; // initial run time = 0
		q1[i].remainingT = q1[i].cpuT; // initially remaining time = burst time
	}
	qsort((void *)q1, n, sizeof(q1[0]), mycomp1); // sort queue1 according to round robin
	clock = q1[0].arrivalT;
	int n1 = n, n2 = 0, n3 = 0, n4 = 0; // number of processes in that queue
	int i = 0, j = 0, k = 0, l = 0;
	while(completed < n) {
		qsort((void *)q4, n4, sizeof(q4[0]), srft_comparator); // For SRTF sort with respect to remaining time
		int ok = 0;
		if(q1[i].arrivalT == clock && q1[i].remainingT != 0) ok = 1; // there is a process to be executed
		if(q1[i].p_id > 0 && q1[i].arrivalT <= clock && !ok) {
			++q1[i].runT;
			--q1[i].remainingT;
			if(q1[i].remainingT <= 0) {
				comp_times[q1[i].p_id] = clock;
				++i; ++completed;
			}
		}
		if(q2[j].p_id > 0) {
			++q2[j].runT;
			--q2[j].remainingT;
			if(q2[j].remainingT <= 0) {
				comp_times[q2[j].p_id] = clock;
				++j; ++completed;
			}
		}
		if(q3[k].p_id > 0) {
			++q3[k].runT;
			--q3[k].remainingT;
			if(q3[k].remainingT <= 0) {
				comp_times[q3[k].p_id] = clock;
				++k; ++completed;
			}
		}
		if(q4[l].p_id > 0) {
			++q4[l].runT;
			--q4[l].remainingT;
			if(q4[l].remainingT <= 0) {
				comp_times[q4[l].p_id] = clock;
				++l; ++completed;
			}
		}
		if(q1[i].p_id > 0 && q1[i].runT == quant1) {
			memcpy(&q2[n2], &q1[i], sizeof(proc));
			q2[n2].runT = 0;
			++i; ++n2;
		}
		if(q2[j].p_id > 0 && q2[j].runT == quant2) {
			memcpy(&q3[n3], &q2[j], sizeof(proc));
			q3[n3].runT = 0;
			++j; ++n3;
		}
		if(q3[k].p_id > 0 && q3[k].runT == quant3) {
			memcpy(&q4[n4], &q3[k], sizeof(proc));
			q4[n4].runT = 0;
			++k; ++n4;
		}
		++clock;
	}
	for(int i = 1; i <= n; i++) {
		printf("%lld ", comp_times[i]);
	}
	return 0;
}