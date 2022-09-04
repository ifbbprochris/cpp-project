#include<iostream>
#include<thread>
#include<stdio.h>
#include<errno.h>

int main()
{
  pthread_mutex_t my_mutex;
  pthread_mutex_init(&my_mutex, nullptr);

  int ret = pthread_mutex_lock(&my_mutex);

  ret = pthread_mutex_destroy(&my_mutex);
  if (ret != 0)
  {
    if (ret == EBUSY)
      printf("BUSY\n");
    printf("Faild to destory mutex\n");
  }

  ret = pthread_mutex_unlock(&my_mutex);
  ret = pthread_mutex_destroy(&my_mutex);

  if (ret == 0)
    printf("Succeed to destory mutex\n");
  return 0;
}