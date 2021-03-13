#include <iostream>
#include <string>
#include <cstdint>
using namespace std;

#include <boost/bimap.hpp>
#include <boost/flyweight.hpp>
#include <boost/flyweight/key_value.hpp>
using namespace boost;
using namespace flyweights;

/**
 * 플라이웨이트는 대단히 많은 수의 매우 비슷한 객체들이 사용되어 할 때 메모리 사용량을 절감하는 방법으로서 자주 사용된다.
 */

// coloring in the console by-letter vs using ranges

// boost.flyweight  

// naive
/**
 * 포인터 대신 인덱스를 사용하면 메모리를 더 절약할 수 있다 8바이트 -> 4바이트
 */
typedef uint32_t key;

// mmorpg
struct User
{
  User(const string& first_name, const string& last_name)
    : first_name{add(first_name)}, last_name{add(last_name)}
  {
  }

  const string& get_first_name() const
  {
    return names.left.find(last_name)->second;
  }

  const string& get_last_name() const
  {
    return names.left.find(last_name)->second;
  }

  static void info()
  {
    for (auto entry : names.left)
    {
      cout << "Key: " << entry.first << ", Value: " << entry.second << endl;
    }
  }

  friend ostream& operator<<(ostream& os, const User& obj)
  {
    return os
      << "first_name: " << obj.first_name << " " << obj.get_first_name()
      << " last_name: " << obj.last_name << " " << obj.get_last_name();
  }

protected:
  static bimap<key, string> names;
  static int seed;

  static key add(const string& s)
  {
    auto it = names.right.find(s);
    if (it == names.right.end())
    {
      // add it
      key id = ++seed;
      names.insert(bimap<key, string>::value_type(seed, s));
      return id;
    }
      //만약 존재한다면 인덱스를 리턴하여 초기화 리스트에서 사용함
    return it->second;
  }
  key first_name, last_name;
};

int User::seed = 0;
bimap<key, string> User::names{};

void naive_flyweight()
{
  User john_doe{ "John", "Doe" };
  User jane_doe{ "Jane", "Doe" };

  cout << "John " << john_doe << endl;
  cout << "Jane " << jane_doe << endl;

  User::info();
}

/**
 * 위와 같은 caching은 보편적으로 사용되는 것이어서 부스트 라이브러리에도 있다.
 * bimap을 사용하지 않고 아래와 같이 부스트 라이브러리를 활용하면 간단히 flyweight를 구현할 수 있다.
 */

struct User2
{
  // users share names! e.g., John Smith
  flyweight<string> first_name, last_name;
  //string first_name, last_name;
  // ...

  User2(const string& first_name, const string& last_name)
  {
  }
};

void boost_flyweight()
{
  User2 john_doe{ "John", "Doe" };
  User2 jane_doe{ "Jane", "Doe" };

  
  cout << boolalpha <<  (&jane_doe.last_name.get() == &john_doe.last_name.get());
  //cout << (&jane_doe.last_name == &john_doe.last_name);
}

int main_()
{
  naive_flyweight();
  boost_flyweight();

  getchar();
  return 0;
}
