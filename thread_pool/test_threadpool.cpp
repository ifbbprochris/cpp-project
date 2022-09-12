#include  "thread_pool.hpp"
#include <chrono>

int main()
{
  TaskPool taskPool;
  taskPool.init();

  Task* task = nullptr;
  for (int i = 0; i < 10; i++)
  {
    task = new Task();
    taskPool.addTask(task);
  }

  std::this_thread::sleep_for(std::chrono::seconds(10));

  taskPool.stop();

  return 0;
}