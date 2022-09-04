#include <iostream>
#include <thread>
#include <semaphore.h>
#include <unistd.h>
#include <list>
#include <errno.h>

class Task
{
public:
  Task(int taskID)
  {
    this->taskID = taskID;
  }

  void doTask()
  {
    std::cout << "handle a task, taskID: " << taskID << ", threadID: " << pthread_self() <<std::endl;
  }
private:
  int taskID;
};

pthread_mutex_t my_mutex;
std::list<Task*> tasks;
sem_t my_semaphore;

void* consumer_thread(void* arg)
{
  Task* ptask = nullptr;

  while (true)
  {
    if (sem_wait(&my_semaphore) != 0)
      continue;

    if (tasks.empty())
      continue;

    pthread_mutex_lock(&my_mutex);
    ptask = tasks.front();

    tasks.pop_front();
    pthread_mutex_unlock(&my_mutex);

    ptask->doTask();
    delete ptask;
  }

  return nullptr;
}

void* producer_thread(void* arg)
{
  int taskID = 0;
  Task* ptask = nullptr;

  while(true)
  {

    ptask = new Task(taskID);
    pthread_mutex_lock(&my_mutex);
    tasks.push_back(ptask);

    std::cout << "produce a task, taskID: " << taskID << ", threadID: " << pthread_self() << std::endl;
    pthread_mutex_unlock(&my_mutex);
    sem_post(&my_semaphore);
    taskID++;

    sleep(1);
  }

  return nullptr;
}

int main()
{

  pthread_mutex_init(&my_mutex, nullptr);

  sem_init(&my_semaphore, 0, 0);

  pthread_t consumerThreadID[5];
  for (int i = 0; i < 5; i++)
    pthread_create(&consumerThreadID[i], nullptr, consumer_thread, nullptr);
  
  pthread_t producerID;
  pthread_create(&producerID, nullptr, producer_thread, nullptr);

  pthread_join(producerID, nullptr);
 
  for (int i = 0; i < 5; i++)
    pthread_join(consumerThreadID[i], nullptr);
  

  sem_destroy(&my_semaphore);
  pthread_mutex_destroy(&my_mutex);

  return 0;
}