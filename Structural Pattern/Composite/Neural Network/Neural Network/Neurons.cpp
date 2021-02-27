#include <cstdio>
#include <vector>
#include <iostream>
#include <boost/predef/library/c.h>
#include <boost/predef/library/c.h>
#include <boost/predef/library/c.h>
#include <boost/predef/library/c.h>
using namespace std;

struct Neuron;

/**
 * 베이스 클래스에 연결 함수를 만들고, 다중 상속을 이용한다.
 */
template <typename Self>
struct SomeNeurons
{
    /**
     * 지정받은 타입 T의 인자 other에 대해 *this를 순회하며 other의 뉴런을 연결한다.
     * *this는 SomeNeurons& 타입이기 때문에 그냥 순회할 수 가 없다. 따라서 실제 타입 Self로 static cast
     * 실제 타입 Self는 나중에 구현 클래스에서 SomeNeurons를 상속받을 때 템틀릿 인자로 지정된다.
     * 범위 기반 for loop가 동작할 수 있도록 begin(), end()를 Neuron과 NeuronLayer에서 구현하는 것이다.
     */
  template <typename T> void connect_to(T& other)
  {
    for (Neuron& from : *static_cast<Self*>(this))
    {
      for (Neuron& to : other)
      {
        from.out.push_back(&to);
        to.in.push_back(&from);
      }
    }
  }
};

struct Neuron : SomeNeurons<Neuron>
{
  vector<Neuron*> in, out;
    //개별 뉴런을 식별하기 위해
  unsigned int id;

  Neuron()
  {
    static int id = 1; 
    this->id = id++;
  }

  /*template <typename T> void connect_to(T& other)
  {
    for (Neuron& target : other)
      connect_to(target);
  }*/

  // legal in MSVC only
  /*template<> void connect_to<Neuron>(Neuron& other)
  {
    out.push_back(&other);
    other.in.push_back(this);
  }*/

  // connect_to(vector<Neuron>&)

  Neuron* begin() { return this; }
  Neuron* end() { return this + 1; }

  friend ostream& operator<<(ostream& os, const Neuron& obj)
  {
    for (Neuron* n : obj.in)
    {
      os << n->id << "\t-->\t[" << obj.id << "]" << endl;
    }

    for (Neuron* n : obj.out)
    {
      os << "[" << obj.id << "]\t-->\t" << n->id << endl;
    }
    return os;
  }
};

//컨테이너 객체를 상속받는건 매우 좋지 않다!
struct NeuronLayer : vector<Neuron>, SomeNeurons<NeuronLayer>
{
  NeuronLayer(int count)
  {
      //0으로 수렴하는 듯 보이는 효과
    while (count-- > 0)
      emplace_back(Neuron{});
  }

  friend ostream& operator<<(ostream& os, NeuronLayer& obj)
  {
    
    for (auto& n : obj) os << n;
    return os;
  }
};

/**
 * Composite가 없다면, 뉴런 대 뉴런, 뉴런 대 레이어, 레이어 대 뉴런, 레이어 대 레이어 네가지 연결을 구현해야한다.
 * 카르테시안 프로덕트 만큼 늘어난다.
 */

int main()
{
  Neuron n1, n2;
  n1.connect_to(n2);

  cout << n1 << n2 << endl;

  NeuronLayer l1{5};
  Neuron n3;
  l1.connect_to(n3);
  
  cout << "Neuron " << n3.id << endl << n3 << endl;
  cout << "Layer " << endl << l1 << endl;

  NeuronLayer l2{ 2 }, l3{ 3 };
  l2.connect_to(l3);
  cout << "Layer l2" << endl << l2;
  cout << "Layer l3" << endl << l3;
}
