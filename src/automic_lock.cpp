#include <iostream>
#include <thread>
#include <chrono>
#include <pthread.h>
#include <unistd.h>
#include <mutex>

#include <sys/time.h>

#include <atomic>

#define TIME_SUB_MS(tv1, tv2)  ((tv1.tv_sec - tv2.tv_sec) * 1000 + (tv1.tv_usec - tv2.tv_usec) / 1000)

// class spin_mutex {
// public:
//   spin_mutex():
//     flag(ATOMIC_FLAG_INIT){};

//   void lock() {
//     while(flag.test_and_set(std::memory_order_acquire));
//   }

//   void unlock() {
//     flag.clear(std::memory_order_release);
//   }
// private:
//   std::atomic_flag flag;
// };

std::mutex mutex;
std::atomic_flag spinmutex = ATOMIC_FLAG_INIT;

// pthread_mutex_t mutex;

#define THREAD_SIZE 10

int inc(int *value, int add) {

  int old;
  __asm__ volatile (
    "lock; xaddl %2, %1;"
    : "=a" (old)
    : "m" (*value), "a" (add)
    : "cc", "memory"
  );

  return old;
}

void *func(void *arg) {
  int *pcount = (int *)arg;

  int i = 0;
  while (i++ < 1000000) {

#if 0
    mutex.lock();
		(*pcount)++;
		mutex.unlock();


#elif 0
  while(spinmutex.test_and_set(std::memory_order_acquire));
  (*pcount)++;
  spinmutex.clear(std::memory_order_release);

#else 
  inc(pcount, 1);

#endif

    // usleep(1);
    // std::cout <<  "threadID:" << std::this_thread::get_id() << ", count: " << *pcount << std::endl;


   
  }


  return 0;
}

int main() {

  std::thread **thread_array = new std::thread* [THREAD_SIZE];
  int count = 0;

  // pthread_mutex_init(&mutex, NULL);

  struct timeval tv_start;

	gettimeofday(&tv_start, NULL);

#if 0

  std::thread t1(func, &count);
  std::thread t2(func, &count);
  std::thread t3(func, &count);
  std::thread t4(func, &count);

  t1.join();
  t2.join();
  t3.join();
  t4.join();

#elif 0
  for (int i = 0; i < THREAD_SIZE; i++) {
    pthread_create(&threads[i], nullptr,  func, &count);
  }
    
  for (int i = 0; i < THREAD_SIZE; i++) {
    pthread_join(threads[i], nullptr);
  }

#elif 1

  for (int i = 0; i < THREAD_SIZE; i++) {
    thread_array[i] = new std::thread(func, &count);
  }

  for (int i = 0; i < THREAD_SIZE; i++) {
    thread_array[i]->join();
  }

#endif

  struct timeval tv_end;

	gettimeofday(&tv_end, NULL);

	int time_used = TIME_SUB_MS(tv_end, tv_start);
	printf("time_used: %d\n", time_used);


#if 1
  for (int i = 0;i < 100;i ++) {
		printf("count --> %d\n", count);
		sleep(1);
	}
#endif

  return 0;
}