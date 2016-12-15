/*
 *  threadPool.c
 *  lab05lect
 *
 *  Created by AJ Bieszczad on 2/17/09.
 *  Copyright 2009 CSUCI. All rights reserved.
 *
 */

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#if !defined(_GNU_SOURCE)
#define _GNU_SOURCE
#endif
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

/*
 *	<Variable Initialization>
 */

//task structure
struct task
{
   int task_id;
   void (*function_pointer)(void);
   struct task* next;
};

//total tasks
int total_tasks = 0;

//request errors
int request_errors = 0;

//recursive mutex
// DOES NOT COMPILE ON Mac
// pthread_mutex_t task_queue_mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
pthread_mutex_t task_queue_mutex = PTHREAD_MUTEX_INITIALIZER;

//conditional variable
pthread_cond_t active_task = PTHREAD_COND_INITIALIZER;

//thread ID's
int thr_id[];

//thread array
pthread_t p_threads[];

//linked list head
struct task* tasks = NULL;

//last node
struct task* bottom_task = NULL;

/*
 *	</Variable Initializaiton>
 */

/*
 *	<ThreadPool Methods>
 */

void *add_task(int task_num, pthread_mutex_t* mutex, pthread_cond_t*
               cond, void (*fp)(void))
{
   struct task* enqueue_task;
   int rs = 0; //return status
   
   enqueue_task = (struct task*)malloc(sizeof(struct task));
   
   if (!enqueue_task)
   {
      request_errors++;
      return NULL;
   }
   
   enqueue_task->task_id = task_num;
   enqueue_task->function_pointer = fp;
   enqueue_task->next = NULL;
   
   rs = pthread_mutex_lock(mutex);
   
   if (total_tasks == 0)
   {
      tasks = enqueue_task;
      bottom_task = enqueue_task;
   }
   else
   {
      bottom_task->next = enqueue_task;
      bottom_task = enqueue_task;
   }
   
   total_tasks++;
   
   rs = pthread_mutex_unlock(mutex);
   
   rs = pthread_cond_signal(cond);
   
   return NULL;
}

struct task* get_task(pthread_mutex_t* mutex)
{
   int rs;
   struct task* task;
   
   rs = pthread_mutex_lock(mutex);
   
   if (total_tasks > 0)
   {
      task = tasks;
      tasks = task->next;
      
      if (tasks == NULL)
      {
         bottom_task = NULL;
      }
      total_tasks--;
   }
   else
   {
      task = NULL;
   }
   
   rs = pthread_mutex_unlock(mutex);
   
   return task;
}

void execute_task(struct task* task, int thread_id)
{
   if (task)
   {
      
   }
}

void* handle_requests_loop(void* data)
{
   int rs;
   struct task* task;
   int thread_id = *((int*)data);
   
   rs = pthread_mutex_lock(&task_queue_mutex);
   
   while (1)
   {
      if (total_tasks > 0)
      {
         task = get_task(&task_queue_mutex);
         if (task)
         {
            rs = pthread_mutex_unlock(&task_queue_mutex);
            execute_task(task, thread_id);
            free(task);
            rs = pthread_mutex_lock(&task_queue_mutex);
         }
         else
         {
            rs = pthread_cond_wait(&active_task, &task_queue_mutex);
         }
      }
   }
}

int thread_pool_init(int num_threads)
{
   int iterator;
   
   for (iterator = 0; iterator < num_threads; iterator++)
   {
      thr_id[iterator] = iterator;
      pthread_create(&p_threads[iterator], NULL, handle_requests_loop,
                     (void *)&thr_id[iterator]);
   }
   return 0;
}
/*
 *	</ThreadPool Methods>
 */

void func()
{
   printf("Hello\n");
}

int main()
{
   thread_pool_init(8);
   int i;
   void (*x)(void);
   x = func;
   for (i = 0; i < 11; i++)
   {
      add_task(i, &task_queue_mutex, &active_task, *x);
      
   }
   
   return 0;
}
