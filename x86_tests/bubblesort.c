/*
 * This is a test method for making a bubble sort of structures
 *
 * @author Hans Reichenbach
 */

#include "bubblesort.h"

int debug_enabled2 = 1;





void fun1(void* str) {if(debug_enabled2) printf("%s\n",(char*)str);}
void fun2(void* str) {if(debug_enabled2) printf("%s\n",(char*)str);}
void fun3(void* str) {if(debug_enabled2) printf("%s\n",(char*)str);}
void fun4(void* str) {if(debug_enabled2) printf("%s\n",(char*)str);}

int main(int argc, char** argv)
{
    puts("TEST STARTING...");

    task_t tasks[TASK_NO];
	tasks[0].lambda = fun2;
	tasks[0].data = (void*)'2';
	tasks[0].stack_pos = (void*)0xa2000000;
	tasks[0].C = 40;
	tasks[0].T = 100;
	tasks[0].B = 0;

	tasks[1].lambda = fun3;
	tasks[1].data = (void*)'3';
	tasks[1].stack_pos = (void*)0xa1000000;
	tasks[1].C = 40;
	tasks[1].T = 150;
	tasks[1].B = 40;

    tasks[2].lambda = fun4;
	tasks[2].data = (void*)'4';
	tasks[2].stack_pos = (void*)0xa1500000;
	tasks[2].C = 100;
	tasks[2].T = 350;
	tasks[2].B = 0;

	sort_per(tasks, TASK_NO);

	int ret;

	//run ub test
	ret = ub_test(tasks, TASK_NO);

	//check for unsure result
	if(ret < 0) {
		ret = rt_test(tasks, TASK_NO);

		if(debug_enabled2) printf("rt test result = %d\n", ret);
	}

	printf("schulability test result = %d\n", ret);


	return ret;
}







//sort the tasks
void sort_per(task_t* tasks, size_t num_tasks) {
	int swap = 1;

	//until there was no swap
	while(swap == 1) {
		//reset swap
		swap = 0;

		//bubble through the array
		size_t i;
		for(i = 0; i < (num_tasks - 1); i++) {
			task_t a = tasks[i];
			task_t b = tasks[i+1];

			//if a has higher period than b, swap them
			if(a.T > b.T) {
				tasks[i] = b;
				tasks[i+1] = a;

				swap = 1;
			}
		}
	}
}






/**
 * runs the ub test on a task array of given length
 *
 * assumes the array is already sorted by period
 * returns 1 if number of tasks is less than needed for algorithm
 *
 * returns 1 if succeed, 0 if fail, and -1 if unsure
 */
int ub_test(task_t* tasks, size_t num_tasks) {
	if(debug_enabled2) printf("~~~~~~~~~~start ub_test\n");

	if(num_tasks < 2) {
		if(debug_enabled2) printf("~~~~~~~~~~returning from ub_test with ret = %d\n", 1);
		return 1;
	}

	//calculate U(n) = n(2^(1/n) - 1)
	float kroot = kroot2(num_tasks);
	if(kroot < 0) return 0;		// check for root not found error
	double un = num_tasks * (kroot - 1);

	if(debug_enabled2) printf("un = %.5f\n", un);

	//calculate U = (sum from 1-(n-1) over (Ci/Ti))? + (Cn + Bn)/Tn
	double u = 0.0;
	size_t i;
	for(i = 0; i < num_tasks - 1; i++) {
		u = u + ((float)tasks[i].C / (float)tasks[i].T);
	}

	u += ( ((float)tasks[num_tasks-1].C) + tasks[num_tasks-1].B ) / ( (float)tasks[num_tasks-1].T );

	if(debug_enabled2) printf("u = %.5f\n", u);

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

	if(debug_enabled2) printf("~~~~~~~~~~returning from ub_test with ret = %d\n", ret);

	return ret;
}







/*
 * runs an RT test on a task list. Assumes it's already sorted by period.
 *
 * returns 1 if scheduling would succeed, 0 if fail
 */
int rt_test(task_t* tasks, size_t num_tasks) {
	if(debug_enabled2) printf("..........starting rt_test\n");

	size_t cur, prev;

	//calculate for each task
	size_t i;
	for(i = 0; i < num_tasks; i++) {
		if(debug_enabled2) printf("starting loop # %lu\n", i);

		//reset cur and prev
		cur = 0;
		prev = 0;

		//see if we can use ub test for this round
		int ub = ub_test(tasks, i+1);

		if(ub == 1) continue;
		else if(ub == 0) return 0;

		if(debug_enabled2) printf("==========actually using rt test for this round\n");

		//calculate a0
		size_t j;
		cur = tasks[i].B;
		for(j = 0; j <= i; j++) {
			cur += tasks[j].C;
		}

		if(debug_enabled2) printf("a0 = %lu\n", cur);

		//keep calculating until values converge
		while(cur != prev) {
			//save previous to check for convergence
			prev = cur;

			if(debug_enabled2) printf("...\ninside convergence loop::prev = %lu\n", prev);

			/* calculate an
			 * Bi + Ci + sum<k=1 to i-1> ( ceil(prev/Tk) * Ck )
			 */
			cur = tasks[i].B + tasks[i].C;
			size_t k;
			for(k = 0; k < i; k++) {
				int ceil = prev / tasks[k].T;
				double a = (float)prev / (float)tasks[k].T;

				if(a > (double)ceil) ceil += 1;

				cur += ceil * tasks[k].C;
			}

			if(debug_enabled2) printf("inside convergence loop::cur = %lu\n...\n", cur);
		}

		if(debug_enabled2) printf("converged result = %lu\n", cur);
		if(debug_enabled2) printf("T = %lu\n", tasks[i].T);
		if(debug_enabled2) printf("==========ending rt test loop\n");

		//if result > T then return fail
		if(cur > tasks[i].T) {
			if(debug_enabled2) printf("..........returning from rt test with failure\n");
			return 0;
		}
	}

	if(debug_enabled2) printf("..........returning from rt test with success\n");

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
