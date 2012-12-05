
/** @file ub_test.c
 *
 * @brief The UB Test for basic schedulability
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-20
 */

//#define DEBUG 0

#include <sched.h>
#ifdef DEBUG
#include <exports.h>
#endif


/**
 * @brief Perform UB Test and reorder the task list.
 *
 * The task list at the end of this method will be sorted in order of priority
 * -- from highest priority (lowest priority number) to lowest priority
 * (highest priority number).
 *
 * @param tasks  An array of task pointers containing the task set to schedule.
 * @param num_tasks  The number of tasks in the array.
 *
 * @return 0  The test failed.
 * @return 1  Test succeeded.  The tasks are now in order.
 */
int assign_schedule(task_t** tasks, size_t num_tasks)
{
	//sort by period
	sort_per(tasks, num_tasks);

	//calculate U(n) = n(2^(1/n) - 1)
	float kroot = kroot2(num_tasks);
	if(kroot < 0) return 0;		// check for root not found error
	float un = num_tasks * (kroot - 1);

	//calculate U = (sum from 1-(n-1) over (Ci/Ti))? + (Cn + Bn)/Tn
	float u = 0;
	size_t i;
	for(i = 0; i < num_tasks - 1; i++) {
		u += tasks[i]->C / tasks[i]->T;
	}
	u += (tasks[num_tasks-1]->C + tasks[num_tasks-1]->B) / tasks[num_tasks-1]->T;

	//figure out if schedulable
	//0 < U <= U(n)		success
	//U(n) < U <= 1.00	not sure
