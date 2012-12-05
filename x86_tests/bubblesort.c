/*
 * This is a test method for making a bubble sort of structures
 *
 * @author Hans Reichenbach
 */

#include "bubblesort.h"

int debug_enabled2 = 1;





void fun1(void* str) {printf("%s\n",(char*)str);}
void fun2(void* str) {printf("%s\n",(char*)str);}
void fun3(void* str) {printf("%s\n",(char*)str);}
void fun4(void* str) {printf("%s\n",(char*)str);}

int main(int argc, char** argv)
{
    puts("TEST STARTING...");

    task_t tasks[TASK_NO];
	tasks[0].lambda = fun2;
	tasks[0].data = (void*)'2';
	tasks[0].stack_pos = (void*)0xa2000000;
	tasks[0].C = 20;
	tasks[0].T = 100;

	tasks[1].lambda = fun3;
	tasks[1].data = (void*)'3';
	tasks[1].stack_pos = (void*)0xa1000000;
	tasks[1].C = 40;
	tasks[1].T = 150;

    tasks[2].lambda = fun4;
	tasks[2].data = (void*)'4';
	tasks[2].stack_pos = (void*)0xa1500000;
	tasks[2].C = 100;
	tasks[2].T = 350;

	sort_per(tasks, TASK_NO);

	int ret;

	//run ub test
	ret = ub_test(tasks, TASK_NO);

	printf("ub test result = %d\n", ret);

	//check for unsure result
	if(ret < 0) {
		ret = rt_test(tasks, TASK_NO);

		printf("rt test result = %d\n");
	}


	return 1;
}







//sort the tasks
void sort_per(task_t* tasks, size_t num_tasks) {
	int swap = 1;

	if(debug_enabled2 == 1) printf("ub_test::sort start\n");
	if(debug_enabled2 == 1) printf("ub_test::sort num_tasks = %lu\n", num_tasks);

	//until there was no swap
	while(swap == 1) {
		//reset swap
		swap = 0;

		//bubble through the array
		size_t i;
		for(i = 0; i < (num_tasks - 1); i++) {
			if(debug_enabled2 == 1) printf("ub_test::sort loop %lu\n", i);

			task_t a = tasks[i];
			task_t b = tasks[i+1];

			if(debug_enabled2 == 1) printf("ub_test::sort a.T = %lu\n", a.T);
			if(debug_enabled2 == 1) printf("ub_test::sort b.T = %lu\n", b.T);

			//if a has higher period than b, swap them
			if(a.T > b.T) {
				if(debug_enabled2 == 1) printf("ub_test::sort swap elements~~~~~~~~~\n");

				tasks[i] = b;
				tasks[i+1] = a;

				if(debug_enabled2 == 1) printf("ub_test::sort tasks[i].T = %lu\n", tasks[i].T);
				if(debug_enabled2 == 1) printf("ub_test::sort tasks[i+1].T = %lu\n", tasks[i+1].T);

				swap = 1;

				if(debug_enabled2 == 1) printf("ub_test::sort done swapping~~~~~~~~~\n");
			}
		}
	}
}






/**
 * runs the ub test on a task array of given length
 *
 * assumes the array is already sorted by period
 *
 * returns 1 if succeed, 0 if fail, and -1 if unsure
 */
int ub_test(task_t* tasks, size_t num_tasks) {
	printf("start ub_test~~~~~~~~~\n");

	//calculate U(n) = n(2^(1/n) - 1)
	float kroot = kroot2(num_tasks);
	if(kroot < 0) return 0;		// check for root not found error
	float un = num_tasks * (kroot - 1);

	printf("un = %.5f\n", un);

	//calculate U = (sum from 1-(n-1) over (Ci/Ti))? + (Cn + Bn)/Tn
	float u = 0;
	size_t i;
	for(i = 0; i < num_tasks - 1; i++) {
		u += tasks[i].C / tasks[i].T;
	}
	u += (tasks[num_tasks-1].C /*+ tasks[num_tasks-1].B*/) / tasks[num_tasks-1].T;

	printf("u = %.5f\n", u);

	//figure out if schedulable
	//0 < U <= U(n)		success
	//U(n) < U <= 1.00	not sure
	// 1 < U			fail
	int ret;
	if(u > 0 && u <= un) {
		ret = 1;
	} else if(un < u && u <= 1.0) {
		ret = -1;
	} else {
		ret = 0;
	}
	return ret;
}







/*
 * runs an RT test on a task list. Assumes it's already sorted by period.
 *
 * returns 1 if scheduling would succeed, 0 if fail
 */
int rt_test(task_t* tasks, size_t num_tasks) {
	printf("starting rt_test........\n");

	size_t cur, prev;

	//calculate for each task
	size_t i;
	for(i = 0; i < num_tasks; i++) {
		printf("starting loop # %lu\n", i);

		//reset cur and prev
		cur, prev = 0;

		//see if we can use ub test for this round
		int ub = ub_test(tasks, num_tasks);

		printf("ub test return = %d\n", ub);

		if(ub == 1) continue;
		else if(ub == 0) return 0;

		printf("actually using rt test for this round==========\n");

		//calculate a0
		size_t j;
		for(j = 0; j < i; j++) {
			cur += tasks[j].C;
		}

		//keep calculating until values converge
		while(cur != prev) {
			//save previous to check for convergence
			prev = cur;

			printf("inside convergence loop::prev = %lu\n", prev);

			/* calculate an
			 * Ci + sum<k=1 to i-1> ( ceil(prev/Tk) * Ck )
			 */
			cur = tasks[i].C;
			size_t k;
			for(k = 0; k < i - 1; k++) {
				cur += ((prev / tasks[k].T) + 1) * tasks[k].C;
			}

			printf("inside convergence loop::cur = %lu\n", cur);
		}

		printf("converged result = %lu\n", cur);
		printf("T = %lu\n", tasks[i].T);

		//if result > T then return fail
		if(cur > tasks[i].T) return 0;
	}

	//if makes it out of hte loop then should be schedulable
	return 1;
}






/*
 * finds the kth root of 2 and returns it
 *
 * returns -1 if invalid input given
 */
float kroot2(size_t k) {
	const float roots[64] = {	2.000000000, 1.414213562, 1.259921050, 1.189207115, 1.148698355,
								1.122462048, 1.104089514, 1.090507733, 1.080059739, 1.071773463,
								1.065041089, 1.059463094, 1.054766076, 1.050756639, 1.047294123,
								1.044273782, 1.041616011, 1.039259226, 1.037155044, 1.035264924,
								1.033557783, 1.032008280, 1.030595545, 1.029302237, 1.028113827,
								1.027018051, 1.026004485, 1.025064212, 1.024189560, 1.023373892,
								1.022611436, 1.021897149, 1.021226606, 1.020595910, 1.020001609,
								1.019440644, 1.018910284, 1.018408093, 1.017931884, 1.017479692,
								1.017049744, 1.016640439, 1.016250325, 1.015878083, 1.015522513,
								1.015182518, 1.014857098, 1.014545335, 1.014246387, 1.013959480,
								1.013683900, 1.013418991, 1.013164143, 1.012918795, 1.012682424,
								1.012454548, 1.012234716, 1.012022510, 1.011817539, 1.011619440,
								1.011427873, 1.011242521, 1.011063084, 1.010889286 };

	//check bounds
	if(k < 1 || k > 64) { return -1; }
	else return roots[k-1];
}
