#include <thread>
#include <iostream>
#include <condition_variable>
#include <mutex>
#include <list>

class Task
{
public:
  Task(int taskID)
  {
    this->taskID = taskID;
  }

  void doTask()
  {
    std::cout << "handle a task, taskID: " << taskID << ", threadID:" << std::this_thread::get_id() << std::endl;
  }

private:
  int taskID;
};

std::mutex mutex;
std::condition_variable cond;
std::list<Task*> tasks;

void* consumer_thread()
{
  Task* task = nullptr;
  while (true)
  {
    std::unique_lock<std::mutex> lock(mutex);
    while (tasks.empty()) 
    {
      cond.wait(lock);
    }

    task = tasks.front();
    tasks.pop_front();

    if (task == nullptr)
      continue;

    task->doTask();
    delete task;
    task = nullptr;
  }
  
  return nullptr;
}

void* producer_thread()
{
  int taskID = 0;
  Task* task = nullptr;

  while (true)
  {
    task = new Task(taskID);

    {
      std::lock_guard<std::mutex> guard(mutex);

      tasks.push_back(task);
      std::cout << "producer a task, taskID:" << taskID << ", threadID:" << std::this_thread::get_id() << std::endl;
    }

    cond.notify_one();

    taskID++;

    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  return nullptr;
}

int main()
{
  std::thread c1(consumer_thread);
  std::thread c2(consumer_thread);
  std::thread c3(consumer_thread);
  std::thread c4(consumer_thread);
  std::thread c5(consumer_thread);

  std::thread p1(producer_thread);

  p1.join();
  c1.join();
  c2.join();
  c3.join();
  c4.join();
  c5.join();

  return 0;
}