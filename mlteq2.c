// https://www.codechef.com/OSP32020/problems/MLTEQ
// Solve the Equation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>

typedef struct clean_input {
	char eq[10000];
	int input_len;
	int pos_op[10000];
	int num_ops;
	int map[26];
	int num_operands;
} clean_input;

typedef struct threadstr {
	clean_input* cleaninp;
	int lt_index;
	int rt_index;
	double sum;
} threadstr;

int sign_symb(char op) {
	return (op == '-') ? -1 : (op == '+') ? 1 : 0;
}

void getop(clean_input *cleaninp) {
	for(int i = 0; i < strlen(cleaninp->eq); i++) {
		if(cleaninp->eq[i - 1] == ')' && sign_symb(cleaninp->eq[i]) && cleaninp->eq[i + 1] == '(') {
			cleaninp->pos_op[cleaninp->num_ops] = i;
			++ cleaninp->num_ops;
		}
	}
	return;
}

double calc(threadstr *eq, int st, int en) {
	int s = 1, stackPtr = -1;
	int val_stack[10000];
	double sum = 0.0;
	for(int i = st; i <= en; i++) {
		if(eq->cleaninp->eq[i] >= 'a' && eq->cleaninp->eq[i] <= 'z') {
			val_stack[++stackPtr] = s * eq->cleaninp->map[eq->cleaninp->eq[i]-'a'];
			s = 1;
		}
		if(eq->cleaninp->eq[i] == '-') s = -1;
		else if(eq->cleaninp->eq[i] == '+') s = 1;
		else if(eq->cleaninp->eq[i] == '^') {
			int sum_param = 0;
			while(stackPtr >= 0) {
				sum_param += val_stack[stackPtr];
				--stackPtr;
			}
			int pow_num = eq->cleaninp->eq[i + 2] - '0';
			int pow_den = 1;
			if(eq->cleaninp->eq[i + 3] == '/') pow_den = eq->cleaninp->eq[i + 4] - '0';
			double exponent = pow_num * 1.00 / pow_den * 1.00;
			sum = pow(sum_param, exponent);
		}
	}
	while(stackPtr >= 0) --stackPtr;
	// printf("Sum = %d\n", (int)eq->sum);
	return sum;
}

void *func(void *arg) {
	threadstr *eq = (threadstr *)arg;
	int start_index = eq->lt_index; // index of open bracket
	int s = 1; // for sign
	for(int i = eq->lt_index; i <= eq->rt_index; i++) {
		if(eq->cleaninp->eq[i] == ')' && sign_symb(eq->cleaninp->eq[i+1]) && eq->cleaninp->eq[i + 2] == '(') {
			eq->sum += s*calc(eq, start_index, i);
			start_index = i+2;
			s = sign_symb(eq->cleaninp->eq[i+1]);
		}
	}
	eq->sum += s*calc(eq, start_index, eq->rt_index);
	// printf("Sum = %d\n", (int)eq->sum);
}

int main() {
	clean_input *cleaninp = (clean_input *)malloc(sizeof(clean_input *));
	cleaninp->num_ops = 0;
	char eq[10000];
	scanf("%[^\n]s", cleaninp->eq);
	cleaninp->input_len = strlen(cleaninp->eq);
	scanf("%d\n", &cleaninp->num_operands);
	for(int i=0; i<cleaninp->num_operands; i++) {
		scanf("%d", &cleaninp->map[i]);
	}
	getop(cleaninp);
	threadstr *lt_side = (threadstr *)malloc(sizeof(threadstr *) * 1);
	lt_side->cleaninp = cleaninp;
	lt_side->lt_index = 0;
	lt_side->rt_index = cleaninp->pos_op[(cleaninp->num_ops - 1)/2]-1;
	lt_side->sum = 0.0;
	threadstr *rt_side = (threadstr *)malloc(sizeof(threadstr *) * 1);
	rt_side->cleaninp = cleaninp;
	rt_side->lt_index = cleaninp->pos_op[(cleaninp->num_ops - 1) / 2] + 1;
	rt_side->rt_index = cleaninp->input_len - 1;
	rt_side->sum = 0.0;
	
	if(cleaninp->eq[cleaninp->pos_op[(cleaninp->num_ops - 1) / 2]] == '-') {
		for(int i = ((cleaninp->num_ops - 1)/2)+1; i<cleaninp->num_ops; i++) {
			cleaninp->eq[cleaninp->pos_op[i]] = (cleaninp->eq[cleaninp->pos_op[i]] == '+')?'-':'+';
		}
	}
	pthread_t t1, t2;
	int rc = pthread_create(&t1, NULL, *func, (void *)lt_side);
	if(rc) {
		printf("Error creating thread 1; rc value = %d\n", rc);
		exit(1);
	}
	rc = pthread_create(&t2, NULL, *func, (void *)rt_side);
	if(rc) {
		printf("Error creating thread 2; rc value = %d\n", rc);
		exit(1);
	}
	pthread_join(t1, NULL); pthread_join(t2, NULL);
	double eval;
	// printf("Left : %d\n", (int)lt_side->sum);
	// printf("Right : %d\n", (int)rt_side->sum);
	eval = lt_side->sum + sign_symb(cleaninp->eq[cleaninp->pos_op[(cleaninp->num_ops - 1) / 2]]) * rt_side->sum;
	printf("%d", (int)eval);
	return 0;
}