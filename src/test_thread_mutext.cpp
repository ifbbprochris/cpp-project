#include <iostream>
#include <thread>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

pthread_mutex_t my_mutex;
int resourceNo = 0;

void *worker_thread(void *arg)
{
  pthread_t tid = pthread_self();
  printf("thread start, thread id is: %ld\n", (long)tid);

  while(1)
  {
    pthread_mutex_lock(&my_mutex);

    printf("thread lock, resourceNo: %d, thread id: %ld\n", resourceNo, (long)tid);
    resourceNo++;
    printf("thread unlock, resourceNo: %d, thread id: %ld\n", resourceNo, (long)tid);

    pthread_mutex_unlock(&my_mutex);

    sleep(1);
  }

  return nullptr;
}

int main()
{
  pthread_mutexattr_t mutex_attr;
  pthread_mutexattr_init(&mutex_attr);
  pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_NORMAL);
  // pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_ERRORCHECK);
  // pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_RECURSIVE);

  pthread_mutex_init(&my_mutex, &mutex_attr);

  // int ret = pthread_mutex_lock(&my_mutex);
  // printf("ret: %d\n", ret);


  pthread_t threadID[5];
  for (int i = 0; i < 5; i++)
    pthread_create(&threadID[i], nullptr, worker_thread, nullptr);

  for (int i = 0; i < 5; i++)
    pthread_join(threadID[i], nullptr);

  pthread_mutex_destroy(&my_mutex);
  pthread_mutexattr_destroy(&mutex_attr);

  return 0;
  
}
