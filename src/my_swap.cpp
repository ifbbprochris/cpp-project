#include<iostream>

using namespace std;

template<class T>
void my_swap(T &a, T &b)
{
  T temp = a;
  a = b;
  b = temp;
}

int main()
{
  int a = 4;
  int b = 5;
  my_swap(a, b);

  cout << a << " , " << b << endl;

  return 0;
}

