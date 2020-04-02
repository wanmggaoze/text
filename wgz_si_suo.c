#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
/*
该历程测试了互斥锁死锁的应用场景
对一个已经上锁了的线程进行第二次上锁，这个线程就会不执行，这个就是死锁
要想正确的执行这个程序，应该把线程1中的互斥锁上锁函数去掉一个
*/
static pthread_mutex_t g_mutex_lock;
static int g_count = 0;

static void *thread_fun_1(void *data)
{
    pthread_mutex_lock(&g_mutex_lock);
	pthread_mutex_lock(&g_mutex_lock);//
    g_count++;
    printf("%s g_count: %d\n", __func__, g_count);

    pthread_mutex_unlock(&g_mutex_lock);
}

static void *thread_fun_2(void *data)
{
    pthread_mutex_lock(&g_mutex_lock);

    g_count++;
    printf("%s g_count: %d\n", __func__, g_count);

    pthread_mutex_unlock(&g_mutex_lock);
}

static void *thread_fun_3(void *data)
{
    pthread_mutex_lock(&g_mutex_lock);

    g_count++;
    printf("%s g_count: %d\n", __func__, g_count);

    pthread_mutex_unlock(&g_mutex_lock);
}

int main(int argc, char const *argv[])
{
    int ret;
    pthread_t pid[3];

    ret = pthread_mutex_init(&g_mutex_lock, NULL);
    if (ret != 0) {
        printf("mutex init failed\n");
        return -1;
    }

    pthread_create(&pid[0], NULL, thread_fun_1, NULL);
    pthread_create(&pid[1], NULL, thread_fun_2, NULL);
    pthread_create(&pid[2], NULL, thread_fun_3, NULL);

    pthread_join(pid[0], NULL);
    pthread_join(pid[1], NULL);
    pthread_join(pid[2], NULL);

    pthread_mutex_destroy(&g_mutex_lock);

    return 0;
}
