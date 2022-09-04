#include <iostream>
#include <memory>

class A : public std::enable_shared_from_this<A>
{
public:
  A()
  {
    m_i = 9;
    std::cout << "A constructor" << std::endl;
  }

  ~A()
  {
    m_i = 0;
    std::cout << "A destructor" << std::endl;
  }

  void func()
  {
    m_selfptr = shared_from_this();
  }
private:
  int m_i;
  std::weak_ptr<A> m_selfptr;
};

int main()
{
  {
    std::shared_ptr<A> spa(new A());
    spa->func();
  }


  return 0;
}