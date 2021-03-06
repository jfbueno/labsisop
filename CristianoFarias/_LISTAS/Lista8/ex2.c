#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>

#define NUM_THREADS 4
#define ROUNDS 10000000;
int cont = 0;

struct WorkUnit
{
	int id;
	int n_runs;
	double v1;
	double v2;
	double result;
};

void *BusyWork(void *param)
{
	int i;
	struct WorkUnit *wu = param;

	printf("Starting thread #%d\n", wu->id);
	wu->result = 0.0;

	switch (cont)
	{
	case 0:
		printf("Starting thread #%d\n", wu->id);
		wu->result = wu->v1 + wu->v2;
		break;
	case 1:
		wu->result = wu->v1 - wu->v2;
		break;
	case 2:
		wu->result = wu->v1 * wu->v2;
		break;
	case 3:
		wu->result = wu->v1 / wu->v2;
		break;
	default:

		break;
	}
	cont++;

	pthread_exit((void *)wu);
}

int main(int argc, char *argv[])
{

	if (argc <= 2)
	{
		printf("Passagem de parametros inválida!\n");
	}
	else
	{

		pthread_t thread[NUM_THREADS];
		struct WorkUnit wunits[NUM_THREADS];
		struct WorkUnit *w;
		struct WorkUnit *status;
		pthread_attr_t attr;
		int rc, t;

		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

		for (t = 0; t < NUM_THREADS; t++)
		{
			w = &wunits[t];
			w->id = t;
			w->n_runs = ROUNDS;
			w->v1 = (double)*argv[1];
			w->v2 = (double)*argv[2];

			printf("Creating thread %d\n", t);

			rc = pthread_create(&thread[t], &attr, &BusyWork, w);
			if (rc)
			{
				errno = rc;
				perror("pthread_create()");
				exit(EXIT_FAILURE);
			}
		} /* for: create */

		pthread_attr_destroy(&attr);

		for (t = 0; t < NUM_THREADS; t++)
		{
			rc = pthread_join(thread[t], (void *)&status);
			if (rc)
			{
				errno = rc;
				perror("pthread_join()");
				exit(EXIT_FAILURE);
			}

			printf("Thread #%d produced result=%f\n", t, status->result);
		} /* for: join */

		pthread_exit(NULL);
	}
} /* main() */
