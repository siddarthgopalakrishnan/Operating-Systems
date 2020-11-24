// https://www.codechef.com/OSP32020/problems/MLTEQ
// Solve the Equation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>

// Stack operations
int stack[10000];
int stackPtr = -1;
void push(int x) {
	stack[++stackPtr] = x;
}
void pop() {
	if(stackPtr != -1) {
		--stackPtr;
	}
}
int isEmpty() {
	return (stackPtr == -1);
}

int top() {
	return stack[stackPtr];
}

char eq[10000];
int param_val[10000];

int calc(int st, int en) {
	printf("Evaluating...");
	for(int tmp = st; tmp <= en; tmp++) {
		printf("%c", eq[tmp]);
	}
	printf("\n");
	int s = 1;
	int ans = 0;
	for(int i=st; i<=en; i++) {
		if(eq[i] >= 'a' && eq[i] <= 'z') {
			push(s * param_val[eq[i] - 'a']);
			s = 1;
		}
		if(eq[i] == '+') s = 1;
		else if(eq[i] == '-') s = -1;
		else if(eq[i] == '^') {
			int param_val = 0;
			while(!isEmpty()) {
				param_val += top();
				pop();
			}
			int pow_num = eq[i+2] - '0';
			int pow_den = 1;
			if(eq[i+3] == '/') pow_den = eq[i+4] - '0';
			double exponent = pow_num*1.00/pow_den*1.00;
			ans = pow(param_val, exponent);
		}
	}
	while(!isEmpty()) {
		// param_val += top();
		pop();
	}
	return ans;
}

void* solve() {
	scanf("%[^\n]s", eq);
	int len = strlen(eq);
	int n;
	scanf("%d", &n);
	for(int i=0; i<n; i++) {
		scanf("%d", &param_val[i]);
	}
	int start_index = 0; // index of open bracket
	int ans = 0; // final answer
	int s = 1; // for sign
	for(int i=0; i<len; i++) {
		if(eq[i] == ')' && eq[i+1] == '+' && eq[i+2] == '(') {
			printf("Prev = %d, End =  %d\n", start_index, i);
			ans += s * calc(start_index, i);
			start_index = i + 2;
			s = 1;
		}
		else if(eq[i] == ')' && eq[i+1] == '-' && eq[i+2] == '(') {
			printf("Prev = %d, End =  %d\n", start_index, i);
			ans += s * calc(start_index, i);
			start_index = i + 2;
			s = -1;
		}
	}
	printf("Prev = %d, End =  %d\n", start_index, len-1);
	ans += s * calc(start_index, len-1);
	printf("%d",ans);
}

int main() {
	pthread_t t1;
	int rc = pthread_create(&t1, NULL, solve, NULL);
	if(rc) {
		printf("Error creating thread; rc value = %d\n", rc);
		exit(1);
	}
	pthread_join(t1, NULL);
	return 0;
}