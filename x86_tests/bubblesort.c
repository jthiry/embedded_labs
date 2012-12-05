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

    task_t tasks[4];
	tasks[0].lambda = fun2;
	tasks[0].data = (void*)'2';
	tasks[0].stack_pos = (void*)0xa2000000;
	tasks[0].C = 1;
	tasks[0].T = 100;

	tasks[1].lambda = fun3;
	tasks[1].data = (void*)'3';
	tasks[1].stack_pos = (void*)0xa1000000;
	tasks[1].C = 1;
	tasks[1].T = 2000;

    tasks[2].lambda = fun4;
	tasks[2].data = (void*)'4';
	tasks[2].stack_pos = (void*)0xa1500000;
	tasks[2].C = 1;
	tasks[2].T = 300;

    //highest priority task
    tasks[3].lambda = fun1;
	tasks[3].data = (void*)'1';
	tasks[3].stack_pos = (void*)0xa2500000;
	tasks[3].C = 1;
	tasks[3].T = 5;

	sort_per(tasks, 4);

	//print out the sorted list
	int i;
	for(i = 0; i < 4; i++) {
		printf("tasks[%d] = %lu\n", i, tasks[i].T);
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
