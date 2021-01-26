#pragma once
#include <string>
#include <iostream>

class PersonBuilder;

class Person
{
  // address <- 여기에 빌더 1개
  std::string street_address, post_code, city;

  // employment <- 여기도 빌더 1개
  std::string company_name, position;
  int annual_income = 0;

  Person()
  {
    std::cout << "Person created\n";
  }
  
public:
  ~Person()
  {
    std::cout << "Person destroyed\n";
  }

  static PersonBuilder create();

    /**
     * Person의 이동 생성자
     * On declaring the new object and assigning it with the r-value, firstly a temporary object is created, and then that temporary object is used to assign the values to the object. Due to this the copy constructor is called several times and increases the overhead and decreases the computational power of the code. To avoid this overhead and make the code more efficient we use move constructors.
     
     */
  Person(Person&& other)
    : street_address{move(other.street_address)},
      post_code{move(other.post_code)},
      city{move(other.city)},
      company_name{move(other.company_name)},
      position{move(other.position)},
      annual_income{other.annual_income}
  {
      std::cout << "Person(Person&& other) called" << std::endl;
  }

    //이동 대입 연산자. 이 예제에서는 안불림!
    // A move constructor is executed only when you construct an object. A move assignment operator is executed on a previously constructed object.
  Person& operator=(Person&& other)
  {
      std::cout << "Person& operator=(Person&& other)" << std::endl;
    if (this == &other)
      return *this;
    
    street_address = move(other.street_address);
    post_code = move(other.post_code);
    city = move(other.city);
    company_name = move(other.company_name);
    position = move(other.position);
    annual_income = other.annual_income;
    return *this;
  }

  friend std::ostream& operator<<(std::ostream& os, const Person& obj)
  {
    return os
      << "street_address: " << obj.street_address
      << " post_code: " << obj.post_code
      << " city: " << obj.city
      << " company_name: " << obj.company_name
      << " position: " << obj.position
      << " annual_income: " << obj.annual_income;
  }

  friend class PersonBuilder;
  friend class PersonAddressBuilder;
  friend class PersonJobBuilder;
};
