//
// Created by root on 18-12-15.
//

#define _GNU_SOURCE

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <sched.h>
#include <linux/unistd.h>
#include <sys/syscall.h>
#include <errno.h>
#include<linux/types.h>
#include<time.h>
#include <sys/time.h>

#define INC_TO 1000000 // one million...

__u64 rdtsc()
{
    __u32 lo,hi;

    __asm__ __volatile__
    (
    "rdtsc":"=a"(lo),"=d"(hi)
    );

    return (__u64)hi<<32|lo;
}



int global_int = 0;
pthread_mutex_t count_lock = PTHREAD_MUTEX_INITIALIZER;


pid_t gettid( void )
{
    return syscall( __NR_gettid );
}

void *thread_routine( void *arg )
{
    int i;
    int proc_num = (int)(long)arg;
    __u64 begin, end;
    struct timeval tv_begin,tv_end;
    __u64 timeinterval;
    cpu_set_t set;

    CPU_ZERO( &set );
    CPU_SET( proc_num, &set );

    if (sched_setaffinity( gettid(), sizeof( cpu_set_t ), &set ))
    {
        perror( "sched_setaffinity" );
        return NULL;
    }

    begin = rdtsc();
    gettimeofday(&tv_begin,NULL);
    for (i = 0; i < INC_TO; i++)
    {
//     global_int++;
        __sync_fetch_and_add( &global_int, 1 );
    }
    gettimeofday(&tv_end,NULL);
    end = rdtsc();
    timeinterval =(tv_end.tv_sec - tv_begin.tv_sec)*1000000                    +(tv_end.tv_usec - tv_begin.tv_usec);
    fprintf(stderr,"proc_num :%d,__sync_fetch_and_add cost             %llu CPU cycle,cost %llu us\n",                             proc_num,end-begin,timeinterval);

    return NULL;
}


void *thread_routine2( void *arg )
{
    int i;
    int proc_num = (int)(long)arg;
    __u64 begin, end;

    struct timeval tv_begin,tv_end;
    __u64 timeinterval;
    cpu_set_t set;

    CPU_ZERO( &set );
    CPU_SET( proc_num, &set );

    if (sched_setaffinity( gettid(), sizeof( cpu_set_t ), &set ))
    {
        perror( "sched_setaffinity" );
        return NULL;
    }


    begin = rdtsc();
    gettimeofday(&tv_begin,NULL);

    for(i = 0;i<INC_TO;i++)
    {
        pthread_mutex_lock(&count_lock);
        global_int++;
        pthread_mutex_unlock(&count_lock);
    }

    gettimeofday(&tv_end,NULL);
    end = rdtsc();


    timeinterval =(tv_end.tv_sec - tv_begin.tv_sec)*1000000                   +(tv_end.tv_usec - tv_begin.tv_usec);
    fprintf(stderr,"proc_num :%d,pthread lock cost %llu CPU                    cycle,cost %llu us\n",proc_num,end-begin                    ,timeinterval);



    return NULL;
}
int main()
{
    int procs = 0;
    int i;
    pthread_t *thrs;

    // Getting number of CPUs
    procs = (int)sysconf( _SC_NPROCESSORS_ONLN );
    if (procs < 0)
    {
        perror( "sysconf" );
        return -1;
    }

    thrs = (pthread_t *)malloc( sizeof( pthread_t ) * procs );
    if (thrs == NULL)
    {
        perror( "malloc" );
        return -1;
    }

    printf( "Starting %d threads...\n", procs );

    for (i = 0; i < procs; i++)
    {
        if (pthread_create( &thrs[i], NULL, thread_routine,
                            (void *)(long)i ))
        {
            perror( "pthread_create" );
            procs = i;
            break;
        }
    }

    for (i = 0; i < procs; i++)
        pthread_join( thrs[i], NULL );

    free( thrs );

    printf( "After doing all the math, global_int value is:              %d\n", global_int );
    printf( "Expected value is: %d\n", INC_TO * procs );

    return 0;
}