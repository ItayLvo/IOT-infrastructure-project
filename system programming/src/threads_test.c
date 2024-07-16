#include <stdio.h>	/* ftupc */
#include <pthread.h>	/* pthread */
#include <unistd.h>	/*sleep*/
#include <time.h>	/* time */
#include <omp.h>

#define ARR_SIZE 100000
#define NUM_THREADS 9

int g_num_arr[ARR_SIZE];

void Run(void);
void *PrintX(void *param);
static void *ThreadAction(void *data);
static size_t FlushThreads(size_t end);


typedef struct info
{
	size_t i;
	size_t sum_res;
} info_t;



int main()
{

	Run();
	
	return 0;
}

void Run(void)
{
	time_t t1 = time(NULL);
	pthread_t thread_arr[ARR_SIZE];
	size_t i = 0, j = 0;
	int status = 0;
/*	for (i = 0; i < ARR_SIZE; ++i)*/
/*	{*/

/*		pthread_create(&thread_arr[i], NULL, *PrintX, &i);*/
/*		pthread_join(thread_arr[i], NULL);*/
/*	}	*/
	
	
/*	while(1)*/
/*	{*/
/*		status = pthread_create(&thread_arr[i], NULL, *PrintX, &i);*/
/*		if (status != 0)*/
/*		{*/
/*			printf("%d\n", status);*/
/*		}*/
/*	}*/
	
	
/*	for (i = 0; i < ARR_SIZE; ++i)*/
/*	{*/
/*		pthread_join(thread_arr[i], NULL);*/
/*	}	*/
	
	
	for (i = 0, j 0; i < NUM_THREADS; ++i, ++j)
	{
		info = (info_t *)malloc(sizeof(info_t));
		
		info->i = i;
		info->sum_res = 0;
	
		status = pthread_create(&thread, NULL, &ThreadAction, info);
		if (0 != status)
		{
			sum_of_divisors += FlushThreads(j);
			--i;
			j = -1;
			free(info);
		}
		else
		{	
			g_thread_arr[j] = thread;		
		}
	}

	
	
	
	
/*	for (i = 0; i < ARR_SIZE; ++i)*/
/*	{*/
/*		printf("%d\n", g_num_arr[i]);*/
/*	}*/
/*	*/
/*	printf("time: %lu\n", time(NULL) - t1);*/
}




static void *ThreadAction(void *data)
{
	size_t exp = ((info_t *)data)->i * (NUMBER / SIZE);
	size_t sum_of_divisors = 0;
	size_t i = 1 + exp;
	size_t end = 0;
	
	if (SIZE - 1 == ((info_t *)data)->i)
	{
		end = NUMBER;
	}
	else
	{
		end = (((info_t *)data)->i + 1) * (NUMBER / SIZE);
	}
	
	for (;i <= end; ++i)
	{
		if (0 == NUMBER % i)
		{
			sum_of_divisors += i;
		}
	}
	
	((info_t *)data)->sum_res = sum_of_divisors;
	
	pthread_exit(data);
}


static size_t FlushThreads(size_t end)
{
	size_t i = 0;
	size_t sum_holder = 0;
	pthread_t *thread_arr_runner = g_thread_arr;
	info_t *info_holder = NULL;
	int status = 0;
	
	
	for (; i < end; ++i, ++thread_arr_runner)
	{
		
		status = pthread_join(*thread_arr_runner, (void **)&info_holder);
		if (0 != status)
		{
			printf("pthread_join Error status %d\n", status);
		}
		
		sum_holder += info_holder->sum_res;
		free(info_holder);
	}
	
	return sum_holder;
}



void *PrintX(void *param)
{
	int x = *(int *)param;
	g_num_arr[x] = x;
	
	return NULL;
}


