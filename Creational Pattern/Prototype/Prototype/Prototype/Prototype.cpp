#include <string>
#include <iostream>
#include <memory>
#include <functional>
#include <sstream>
//#include "Person.h"
using namespace std;
/**
 * Serialization: 어떤 객체 데이터를 비트의 나열로 만들어 파일에 저장하거나 네트워크로 전송할 수 있게 하는 것.
 * 객체를 비트열로 나타내어 온전한 상태로 파일이나 메모리에 쓸 수 있으면, 거꾸로 읽어 들여(deserialize)
 * 모든 정보와 내부 구성 요소들을 복구할 수 있다. 복제작업과 동일!
 * C++에는 기본기능으로 제공하지않아 boost 이용
 */
#include <boost/serialization/serialization.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

/**
 * 처음부터 모두 새로 객체를 만드는 방법 대신 이미 존재하는 객체를 이용하여 복제하여 사용
 * 객체 안에 객체가 있을때 값을 갖는 경우는 드물다. 주로 포인터를 들고있음
 * 객체 복사시에 deepcopy 필요.
 */

struct Address
{
  string street;
  string city;
  int suite;

    Address(){}
    
  Address(const string& street, const string& city, const int suite)
    : street{street},
      city{city},
      suite{suite}
  {
  }

    /**
     * Contact에서 Address복사를 쉽게하기 위한 Copy Constructor
     */
  /*Address(const Address& other)
    : street{other.street},
      city{other.city},
      suite{other.suite}
  {
  }*/

  friend ostream& operator<<(ostream& os, const Address& obj)
  {
    return os
      << "street: " << obj.street
      << " city: " << obj.city
      << " suite: " << obj.suite;
  }
    
private:
  friend class boost::serialization::access;
//저장 address앞에 *가 없다!
    //객체를 구성하는 모든 타입에 serialize()를 구현해야 한다.
  template <class archive>
  void save(archive& ar, const unsigned version) const
  {
    ar << street;
    ar << city;
    ar << suite;
  }
//복구
  template <class archive>
  void load(archive& ar, const unsigned version)
  {
      ar >> street;
      ar >> city;
      ar >> suite;
  }
    // split member function serialize funcition into save/load
  BOOST_SERIALIZATION_SPLIT_MEMBER()
};


struct Contact
{
  string name;
  Address* address;

  Contact& operator=(const Contact& other)
  {
    if (this == &other)
      return *this;
    name = other.name;
    address = other.address;
    return *this;
  }

  Contact()
  {} // required for serialization

  Contact(string name, Address* address)
    : name{name}, address{address}
  {
    //this->address = new Address{ *address };
  }

    /**
     * Copy constructor
     * initialization list에서도 가능한데, 이는 Address에도 copy constructor가 있기 때문이다.(주석 처리 됨)
     * DeepCopy를 위해 new는 꼭 사용해야한다.
     */
  Contact(const Contact& other)
    : name{other.name}
    //, address{ new Address{*other.address} }
  {
      /**
       * 충분히 범용적이지 않음. 주소의 항목이 바뀐다면 문제가 된다.
       */
    address = new Address(
      other.address->street,
      other.address->city,
      other.address->suite
    );
  }


private:
  friend class boost::serialization::access;
//저장 address앞에 *가 없다!
    //객체를 구성하는 모든 타입에 serialize()를 구현해야 한다.
  template <class archive>
  void save(archive& ar, const unsigned version) const
  {
    ar << name;
    ar << address;
  }
//복구
  template <class archive>
  void load(archive& ar, const unsigned version)
  {
    ar >> name;
    ar >> address;
  }
    // split member function serialize funcition into save/load
  BOOST_SERIALIZATION_SPLIT_MEMBER()

public:
  ~Contact()
  {
    delete address;
  }


  friend ostream& operator<<(ostream& os, const Contact& obj)
  {
    return os
      << "name: " << obj.name
      << " works at " << *obj.address; // note the star here
  }
};
/**
 * 프로토타입을 저장할 별도의 클래스를 두고 프로토타입의 사용자가 원할 때,
 * 목적에 맞는 복제본을 요구받는 시점에 만들어 제공
 *
 * (1) 모든 항목을 지정받은 생성자 : public
 * (2) 부분적인 생성자: private + EmployFactory(friend)
 *  => 미완성된 객체 복제본이 돌아다닐 가능성을 원천적으로 차단!
 */
struct EmployeeFactory
{
  
  static unique_ptr<Contact> NewMainOfficeEmployee(string name, int suite)
  {
    static Contact main{ "", new Address{ "123 East Dr", "London", 0 } };
    return NewEmployee(name, suite, main);
  }

  static unique_ptr<Contact> NewAuxOfficeEmployee(string name, int suite)
  {
    static Contact aux{ "", new Address{ "123 West Dr", "London", 0 } };
    return NewEmployee(name, suite, aux);
  }

private:
  static unique_ptr<Contact> NewEmployee(string name, int suite, Contact& proto)
  {
    auto result = make_unique<Contact>(proto);
    result->name = name;
    result->address->suite = suite;
    return result;
  }
};

//Contact EmployeeFactory::main{ "", new Address{ "123 East Dr", "London", 0 } };
//Contact EmployeeFactory::aux{ "", new Address{ "123B East Dr", "London", 0 } };

int main()
{
  // this is tedious
  // Contact john{ "John Doe", new Address{"123 East Dr", "London"} };
  // Contact jane{ "Jane Doe", new Address{"123 East Dr", "London"} };

  auto addr = new Address{ "123 East Dr", "London", 0 /* ? */ };

    /*
  Contact john{ "John Doe", addr };
  john.address->suite = 123;
  //Pointer가 shallow copy되어 jane의 값을 변경하면 john의 값도 바뀐다.
  Contact jane{ "Jane Doe", addr };
  jane.address->suite = 124;

  Contact jane2{ jane }; // deep copy
  jane2.address->suite = 555;

    //copy constructor가 불림.
    Contact temp = john;
    //assignment operator가 불림.
    //pointer를 shallow copy하므로 문제 발생!
    temp = jane2;
    temp.address->suite = 666;
  std::cout << john << std::endl;
  std::cout << jane << std::endl;
  std::cout << jane2 << std::endl;
    std::cout << temp << std::endl;
     */

  // whenever an address is needed, make a copy
  /*Contact john{ "John Doe", new Address{*addr} };
  john.address->suite = 123;

  Contact jane{ "Jane Doe", new Address{*addr} };
  jane.address->suite = 125;

  cout << john << "\n" << jane << endl;*/

  // much better. let's list employees
  //Contact employee{ "Unknown", new Address{"628 Happy St", "Joy", 0} };

  //// we can use this prototype to create john and jane
  //Contact john{ employee };
  //john.name = "John Doe";
  //john.address->suite = 123;

  //Contact jane{ employee };
  //jane.name = "Jane Doe";
  //jane.address->suite = 125;

  //cout << john << "\n" << jane << "\n";

  //delete addr;

  // 4. Boost Serialization

  // too much work in getting the copying working
    auto john = EmployeeFactory::NewAuxOfficeEmployee("John Doe", 123);
    
    auto clone = [](const Contact& c)
    {
        ostringstream oss;
        boost::archive::text_oarchive oa(oss);
        oa << c;
        string s = oss.str();
        
        istringstream iss(oss.str());
        boost::archive::text_iarchive ia (iss);
        Contact result;
        ia >> result;
        return result;
    };
    
    Contact jane = clone(*john);
    jane.name = "Jane";
    cout << *john << endl;
    cout << jane << endl;
    
  
  //auto jane = EmployeeFactory::NewMainOfficeEmployee("Jane Doe", 125);

  //cout << *john << "\n" << *jane << "\n"; // note the stars here

  delete addr;

  getchar();
  return 0;
}
