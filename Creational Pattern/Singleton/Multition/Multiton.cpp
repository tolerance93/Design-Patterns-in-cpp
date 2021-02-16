#include <map>
#include <memory>
#include <iostream>
using namespace std;

enum class Importance
{
  primary,
  secondary,
  tertiary
};

/**
 * singleton의 변형 패턴. 몇가지 종류를 한개만 생성 가능하도록 한다.
 */

template <typename T, typename Key = std::string>
class Multiton
{
public:
  static shared_ptr<T> get(const Key& key)
  {
      /**
       * C++17 feature. if문 안에서 instance 생성하기
       */
    if (const auto it = instances.find(key);
        it != instances.end())
    {
      return it->second;
    }

    auto instance = make_shared<T>();
    instances[key] = instance;
    return instance;
  }
protected:
    /**
     * Multition class 는 instance화가 불가능. static 함수를 통해서만 접근 가능하다.
     */
  Multiton() = default;
  virtual ~Multiton() = default;
private:
  static map<Key, shared_ptr<T>> instances;
};

/**
 * static member 초기화
 */
template <typename T, typename Key>
map<Key, shared_ptr<T>> Multiton<T, Key>::instances;

class Printer
{
public:
  Printer()
  {
    ++Printer::totalInstanceCount;
    cout << "A total of " <<
      Printer::totalInstanceCount <<
      " instances created so far\n";
  }
private:
    //static member variable로 생성되는 수량 표기
  static int totalInstanceCount;
};
int Printer::totalInstanceCount = 0;

int main()
{
  typedef Multiton<Printer, Importance> mt;

  auto main = mt::get(Importance::primary);
  auto aux = mt::get(Importance::secondary);
  auto aux2 = mt::get(Importance::secondary);
}
