//
//  main.c
//  test
//
//  Created by CC on 14-1-17.
//  Copyright (c) 2014年 user. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#include <semaphore.h>
pthread_t ntid;

#define NLOOP 5000

#define NUM 5

/*create pthread*/

//void printids(const char *s)
//{
//    pid_t      pid;
//    pthread_t  tid;
//    
//    pid = getpid();
//    tid = pthread_self();
//    printf("%s pid %u tid %u (0x%x)\n", s, (unsigned int)pid,
//           (unsigned int)tid, (unsigned int)tid);
//}
//
//void *thr_fn(void *arg)
//{
//    printids(arg);
//    return NULL;
//}
//
//int main(void)
//{
//    int err;
//    
//    err = pthread_create(&ntid, NULL, thr_fn, "new thread: ");
//    if (err != 0) {
//        fprintf(stderr, "can't create thread: %s\n", strerror(err));
//        exit(1);
//    }
//    printids("main thread:");
//    sleep(1);
//    
//    return 0;
//}


/*join pthread*/
//void *thr_fn1(void *arg)
//{
//    printf("thread 1 returning\n");
//    return (void *)1;
//}
//
//void *thr_fn2(void *arg)
//{
//    printf("thread 2 exiting\n");
//    pthread_exit((void *)2);
//}
//
//void *thr_fn3(void *arg)
//{
//    while(1) {
//        printf("thread 3 writing\n");
//        sleep(1);
//    }
//}
//
//int main(void)
//{
//    pthread_t   tid;
//    void        *tret;
//    
//    pthread_create(&tid, NULL, thr_fn1, NULL);
//    pthread_join(tid, &tret);
//    printf("thread 1 exit code %d\n", (int)tret);
//    
//    pthread_create(&tid, NULL, thr_fn2, NULL);
//    pthread_join(tid, &tret);
//    printf("thread 2 exit code %d\n", (int)tret);
//    
//    pthread_create(&tid, NULL, thr_fn3, NULL);
//    sleep(3);
//    pthread_cancel(tid);
//    pthread_join(tid, &tret);
//    printf("thread 3 exit code %d\n", (int)tret);
//    
//    return 0;
//}

/*同步*/


//int counter;                /* incremented by threads */
//
//void *doit(void *);
//
//int main(int argc, char **argv)
//{
//    pthread_t tidA, tidB;
//    
//    pthread_create(&tidA, NULL, &doit, NULL);
//    pthread_create(&tidB, NULL, &doit, NULL);
//    
//    /* wait for both threads to terminate */
//    pthread_join(tidA, NULL);
//    pthread_join(tidB, NULL);
//    
//    return 0;
//}
//
//void *doit(void *vptr)
//{
//    int    i, val;
//    
//    /*
//     * Each thread fetches, prints, and increments the counter NLOOP times.
//     * The value of the counter should increase monotonically.
//     */
//    
//    for (i = 0; i < NLOOP; i++) {
//        val = counter;
//        printf("%x: %d\n", (unsigned int)pthread_self(), val + 1);
//        counter = val + 1;
//    }
//    
//    return NULL;
//}

/*mutex*/
//int counter;                /* incremented by threads */
//pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;
//
//void *doit(void *);
//
//int main(int argc, char **argv)
//{
//    pthread_t tidA, tidB;
//    
//    pthread_create(&tidA, NULL, doit, NULL);
//    pthread_create(&tidB, NULL, doit, NULL);
//    
//    /* wait for both threads to terminate */
//    pthread_join(tidA, NULL);
//    pthread_join(tidB, NULL);
//    
//    return 0;
//}
//
//void *doit(void *vptr)
//{
//    int     i, val;
//    
//    /*
//     * Each thread fetches, prints, and increments the counter NLOOP times.
//     * The value of the counter should increase monotonically.
//     */
//    
//    for (i = 0; i < NLOOP; i++) {
//        pthread_mutex_lock(&counter_mutex);
//        
//        val = counter;
//        printf("%x: %d\n", (unsigned int)pthread_self(), val + 1);
//        counter = val + 1;
//        
//        pthread_mutex_unlock(&counter_mutex);
//    }
//    
//    return NULL;
//}


/*condition variable*/


//struct msg {
//    struct msg *next;
//    int num;
//};
//
//struct msg *head;
//pthread_cond_t has_product = PTHREAD_COND_INITIALIZER;
//pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
//
//void *consumer(void *p)
//{
//    struct msg *mp;
//    
//    for (;;) {
//        pthread_mutex_lock(&lock);
//        while (head == NULL)
//            pthread_cond_wait(&has_product, &lock);
//        mp = head;
//        head = mp->next;
//        pthread_mutex_unlock(&lock);
//        printf("Consume %d\n", mp->num);
//        free(mp);
//        sleep(rand() % 5);
//    }
//}
//
//void *producer(void *p)
//{
//    struct msg *mp;
//    for (;;) {
//        mp = malloc(sizeof(struct msg));
//        mp->num = rand() % 1000 + 1;
//        printf("Produce %d\n", mp->num);
//        pthread_mutex_lock(&lock);
//        mp->next = head;
//        head = mp;
//        pthread_mutex_unlock(&lock);
//        pthread_cond_signal(&has_product);
//        sleep(rand() % 5);
//    }
//}
//
//int main(int argc, char *argv[])
//{
//    pthread_t pid, cid;
//    
//    srand(time(NULL));
//    pthread_create(&pid, NULL, producer, NULL);
//    pthread_create(&cid, NULL, consumer, NULL);
//    pthread_join(pid, NULL);
//    pthread_join(cid, NULL);
//    return 0;
//}

/*semaphore*/
int queue[NUM];
sem_t blank_number, product_number;

void *producer(void *arg)
{
    int p = 0;
    while (1) {
        sem_wait(&blank_number);
        queue[p] = rand() % 1000 + 1;
        printf("Produce %d\n", queue[p]);
        sem_post(&product_number);
        p = (p+1)%NUM;
        sleep(rand()%5);
    }
}

void *consumer(void *arg)
{
    int c = 0;
    while (1) {
        sem_wait(&product_number);
        printf("Consume %d\n", queue[c]);
        queue[c] = 0;
        sem_post(&blank_number);
        c = (c+1)%NUM;
        sleep(rand()%5);
    }
}

int main(int argc, char *argv[])
{
    pthread_t pid, cid;
    
    sem_init(&blank_number, 0, NUM);
    sem_init(&product_number, 0, 0);
    pthread_create(&pid, NULL, producer, NULL);
    pthread_create(&cid, NULL, consumer, NULL);
    pthread_join(pid, NULL);
    pthread_join(cid, NULL);
    sem_destroy(&blank_number);
    sem_destroy(&product_number);
    return 0;
}