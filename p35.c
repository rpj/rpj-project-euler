#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <limits.h>

#define printf

typedef enum { false = 0, true = 1 } bool;

#define kNumBlockSize 64

typedef struct __num_block_t {
	int nums[kNumBlockSize];
	int idx;
	pthread_mutex_t mutex;
	struct __num_block_t* next;
} num_block_t;

typedef struct __limits_t {
	int low;
	int hi;
} limits_t;
	
typedef num_block_t* num_block_ref;
typedef limits_t* limits_ref;

static num_block_ref gNumBlock;
static num_block_ref gPrimeBlock;

static num_block_ref __allocate_num_block(num_block_ref prev)
{
	size_t sz = sizeof(struct __num_block_t);
	num_block_ref nblock = (num_block_ref)malloc(sz);
	bzero(nblock, sz);
	
	if (prev) {
		pthread_mutex_lock(&prev->mutex);
		prev->next = nblock;
		pthread_mutex_unlock(&prev->mutex);
	}
	
	int irv = pthread_mutex_init(&nblock->mutex, NULL);
	
	if (irv)
	{
		printf("_allocate_num_block: pthread_mutex_init error %d: %s\n", irv, strerror(irv));
		free(nblock);
		nblock = NULL;
	}
	
	return nblock;
}

static num_block_ref _create_num_block(void)
{
	return __allocate_num_block(NULL);
}

static void _destroy_num_block(num_block_ref nblock)
{
	if (nblock)
	{
		num_block_ref next = nblock;
		int lockrv = 0;

		do {
			if (!(lockrv = pthread_mutex_trylock(&nblock->mutex)))
			{
				next = nblock->next;
				free(nblock);
				nblock = next;
			}
		} while (!lockrv && nblock);
		
		if (lockrv)
			printf("destroy_num_block(0x%lx) failed for mutex_trylock error %d ('%s')\n", 
			 (long int)nblock, lockrv, strerror(lockrv));
	}
}

static inline bool _num_block_is_full(num_block_ref nblock)
{
	return (nblock ? (nblock->idx == kNumBlockSize) : false);
}

static inline bool _append_to_num_block(int num, num_block_ref nblock)
{
	// shouldn't happen if you're using _end_of_num_block
	if (_num_block_is_full(nblock)) return false;
	
	pthread_mutex_lock(&nblock->mutex);
	nblock->nums[nblock->idx++] = num;
	pthread_mutex_unlock(&nblock->mutex);
	
	return true;
}

static num_block_ref _end_of_num_block(num_block_ref nblock)
{
	if (!nblock) return NULL;
	
	num_block_ref next = nblock->next;
	
	while (next)
		next = (nblock = nblock->next)->next;
	
	if (_num_block_is_full(nblock))
		nblock = __allocate_num_block(nblock);
	
	return nblock;
}

__attribute__((constructor)) 
static void __ctor(void)
{
	gNumBlock = _create_num_block();
	gPrimeBlock = _create_num_block();
}

__attribute__((destructor)) 
static void __dtor(void)
{	
	_destroy_num_block(gNumBlock);
	_destroy_num_block(gPrimeBlock);
}

static bool append_to_block(int num, num_block_ref nblock)
{
	num_block_ref end = _end_of_num_block(nblock);
	return (end ? _append_to_num_block(num, end) : false);
}

static int value_at_index(int idx, num_block_ref nblock)
{
	int in_block = (int)(idx / kNumBlockSize);
	int block_idx = idx % kNumBlockSize;
	int c = 0;
	num_block_ref tmp = nblock;
	
	for (; c < in_block && tmp; tmp = tmp->next, c++);
	
	return (tmp ? tmp->nums[block_idx] : INT_MAX);
}

void* run_thread(void* th_arg)
{
	limits_t* iarg = (limits_t*)th_arg;
	int i = 0;
	
	num_block_ref block = _create_num_block();
	printf("thread %x with limits 0x%x:[%d, %d]\n", pthread_self(), th_arg, iarg->low, iarg->hi);
	
	for (i = iarg->low; i < iarg->hi; i++)
	{
		append_to_block(i, block);
		
		if (!(i % 10000))
			printf("thread %x: append_to_block(%d, %x)\n", (int)pthread_self(), i, block);
	}
	
	pthread_exit((void*)block);
}

int launch_threads(int num, limits_ref lims)
{
	int rv = 0;
	int i = 0;
	long status = 0;
	pthread_t* threads = (pthread_t*)malloc(sizeof(pthread_t) * num);
	pthread_attr_t pattr;
	
	pthread_attr_init(&pattr);
	pthread_attr_setdetachstate(&pattr, PTHREAD_CREATE_JOINABLE);
	
	for (; !rv && i < num; i++)
	{	
		if ((rv = pthread_create(&threads[i], &pattr, run_thread, (void*)(lims + i))))
			printf("pthread_create() failed: %d\n", rv);
	}
	
	pthread_attr_destroy(&pattr);
	
	for (i = 0; i < num; i++)
	{
		if ((rv = pthread_join(threads[i], (void*)&status)))
			printf("pthread_join(%d) failed with %d\n", i, rv);
		else
		{
			printf("thread %d exited with status %ld\n", i, status);
			num_block_ref res_block = (num_block_ref)status;
			
			_destroy_num_block(res_block);
		}
	}
	
	free(threads);
	return rv;
}

void run(int num_threads, int upper_lim)
{
	limits_ref lims = (limits_ref)malloc(sizeof(limits_t) * num_threads);
	float th_ratio = 1.0f / (float)num_threads;
	int i = 1;
	limits_t t_limit = { 0, 0 };
	
	for (; i <= num_threads; i++)
	{
		t_limit.low = t_limit.hi;
		t_limit.hi = (int)((float)upper_lim * (th_ratio * (float)i));
		printf("create limits [%d, %d] and copy to %x\n", t_limit.low, t_limit.hi, (lims + (i - 1)));
		//*(lims + (i - 1)) = (int)((float)upper_lim * (th_ratio * (float)i));
		memcpy((lims + (i - 1)), &t_limit, sizeof(limits_t));
	}	
	
	(void)launch_threads(num_threads, lims);
	free(lims);
}

int main(int argc, char** argv)
{	
	int lim = 1e6;
	int th_count = 2;
	
	if (argc > 1)
		lim = atoi(argv[1]);
	
	if (argc > 2)
		th_count = atoi(argv[2]);
		
	printf("run(%d, %d)\n", th_count, lim);
	run(th_count, lim);
	
	pthread_exit(NULL);
	return 0;
}