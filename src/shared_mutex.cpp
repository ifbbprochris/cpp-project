#include <iostream>
#include <mutex>
#include <thread>
#include <shared_mutex>

#define READER_THREAD_COUNT 8
#define LOOP_COUNT 500000

class share_mutex_counter
{
public:
  share_mutex_counter() = default;
  ~share_mutex_counter() = default;

  unsigned int get_counter() const
  {
    std::shared_lock<std::shared_mutex> lock(m_mutex);
    return m_value;
  }

  void increment()
  {
    std::unique_lock<std::shared_mutex> lock(m_mutex);
    m_value++;
  }

  void reset()
  {
    std::unique_lock<std::shared_mutex> lock(m_mutex);
    m_value = 0;
  }

private:
  unsigned int m_value = 0;
  mutable std::shared_mutex m_mutex;
};

class mutex_counter
{
public:
  mutex_counter() = default;
  ~mutex_counter() = default;

  unsigned int get_counter() const
  {
    std::unique_lock<std::mutex> lock(m_mutex);
    return m_value;
  }

  void increment()
  {
     std::unique_lock<std::mutex> lock(m_mutex);
     m_value++;
     std::cout << m_value << std::endl;
  }

  void reset()
  {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_value = 0;
  }

private:
  unsigned int m_value = 0;
  mutable std::mutex m_mutex;

};

void test_shared_mutex()
{
  share_mutex_counter counter;
  int temp;

  auto writer = [&counter]() {
    for (int i = 0; i < LOOP_COUNT; i++) 
    {
      counter.increment();
    }
  };

  auto reader = [&counter, &temp]() {
    for (int i = 0; i < LOOP_COUNT; i++)
    {
      temp = counter.get_counter();
    }
  };

  std::thread** array = new std::thread* [READER_THREAD_COUNT];

  clock_t start = clock();

  for (int  i = 0; i < READER_THREAD_COUNT; i++)
    array[i] = new std::thread(reader);
  
  std::thread tw(writer);

  for (int  i = 0; i < READER_THREAD_COUNT; i++)
    array[i]->join();

  tw.join();

  clock_t end = clock();
  printf("[test_shared_mutex]\n");
  printf("thread count: %d\n", READER_THREAD_COUNT);
  printf("result: %d   cost: %ldms temp: %d \n", counter.get_counter(), end - start, temp);
}

void test_mutex()
{
  mutex_counter counter;
  int temp;

  auto writer = [&counter]() {
    for (int i = 0; i < LOOP_COUNT; i++) 
    {
      counter.increment();
    }
  };

  auto reader = [&counter, &temp]() {
    for (int i = 0; i < LOOP_COUNT; i++)
    {
      temp = counter.get_counter();
    }
  };

  std::thread** array = new std::thread* [READER_THREAD_COUNT];

  clock_t start = clock();

  for (int  i = 0; i < READER_THREAD_COUNT; i++)
    array[i] = new std::thread(reader);
  
  std::thread tw(writer);

  for (int  i = 0; i < READER_THREAD_COUNT; i++)
    array[i]->join();

  tw.join();

  clock_t end = clock();
  printf("[test_mutex]\n");
  printf("thread count: %d\n", READER_THREAD_COUNT);
  printf("result: %d   cost: %ldms temp: %d \n", counter.get_counter(), end - start, temp);
}


int main()
{
  test_mutex();
  // test_shared_mutex();

  return 0;
}