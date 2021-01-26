#pragma once
#include "Person.h"

class PersonAddressBuilder;
class PersonJobBuilder;

class PersonBuilderBase
{
protected:
    /**
     * 현재 생성되고 있는 객체에 대한 참조. 하위 클래스에서 복사되는 것 방지
     * 하위 빌더들을 위해 일부러 만들어짐.
     * Person의 실제 저장소는 여기가 아니다!
     */
  Person& person;
    /**
     * 참조 대입은 protected이므로 자식 클래스에서만 이용할 수 있다.
     */
  explicit PersonBuilderBase(Person& person)
    : person{ person }
  {
  }
public:
    /**
     * Person이 이동생성자를 올바르게 정의하고 있어야 한다.
     * conversion operator
     */
  operator Person() const
  {
      std::cout << "Person() called" << std::endl;
    return std::move(person);
  }

  // builder facets

  PersonAddressBuilder lives() const;
  PersonJobBuilder works() const;
};

class PersonBuilder : public PersonBuilderBase
{
  Person p; // 생성 중인 객체
public:
  PersonBuilder(): PersonBuilderBase{p}
  {
  }

};
