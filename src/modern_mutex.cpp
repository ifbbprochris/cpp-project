#include <iostream>
#include <thread>
#include <mutex>

int g_num = 0;
std::mutex  g_mutex;

void *increment(void * arg)
{
  for (int i = 0; i < 10; i++)
  {
    g_mutex.lock();
    std::cout << "threadID: " << pthread_self() <<  ", num: " << g_num << std::endl;
    g_num++;
    g_mutex.unlock();

    std::this_thread::sleep_for(std::chrono::seconds(2));
  }

  return nullptr;
}

int main()
{
  std::thread t1(increment, nullptr);
  std::thread t2(increment, nullptr);
  t1.join();
  t2.join();


  return 0;
}