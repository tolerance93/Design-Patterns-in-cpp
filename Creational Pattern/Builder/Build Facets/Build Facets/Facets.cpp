#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <string>
using namespace std;

#include "Person.h"
#include "PersonBuilder.h"
#include "PersonAddressBuilder.h"
#include "PersonJobBuilder.h"

/**
 * 하나의 객체를 생성하는데 복수의 빌더가 사용되는 경우
 */

int main()
{
    /**
     * create():  PersonBuilder를 기본생성자로 생성. 가지고 있는 Person을 참조로 Base에 전달,
     * lives()는 PersonAddressBuilder{person} 리턴 -> PersonBuilderBase에 참조 입력
     * (1) 등호는 생성된 PersonJobBuilder를 rvalue로 바꿈
     * (2) conversion operator 호출
     * (3) 이동 생성자 호출
     * Q.  중간 생성물은 어디로 가는 걸까?
     */
  Person p = Person::create()
    .lives().at("123 London Road").with_postcode("SW1 1GB").in("London")
    .works().at("PragmaSoft").as_a("Consultant").earning(10e6);

  cout << p << endl;
  getchar();
  return 0;
}
