#include "Person.h"

struct Person::PersonImpl
{
  void greet(Person* p);
};

void Person::PersonImpl::greet(Person* p)
{
  printf("hello %s", p->name.c_str());
}

Person::Person()
  : impl(new PersonImpl)
{
}

Person::~Person()
{
  delete impl;
}

void Person::greet()
{
    /**
     * 구현부를 포인터로 참조하는 관례
     */
  impl->greet(this);
}


