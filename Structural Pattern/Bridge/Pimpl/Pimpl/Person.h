#pragma once
#include <string>

/**
 * Pimpl을 왜 사용하는가?
 * 1. 클래스 구현부의 상당 부분이 실제로 감춰질 수 있다: private/protected 멤버를 헤더파일에 안만들어도 된다.
 * 2. 바이너리 호환성을 보증하기 쉬워진다.
 * 3. 헤더 파일이 멤버 선언에 필요한 헤더들만 include하게 할 수 있다. 즉 헤더들을 .cpp로 옮길 수 있다.
 */

struct Person
{
  std::string name;

    /**
     * forward-declare
     * 구현 클래스 PersonImpl을 헤더파일이 아닌 .cpp파일에 정의하는 것이 핵심
     */
  class PersonImpl;
    /**
     * Bridge!
     * 공개용 인터페이스와 숨겨야할 .cppㅏ일의 세부 구현을 연결하는 브릿지 역할!
     */
  PersonImpl *impl; // bridge - not necessarily inner class, can vary

  Person();
  ~Person();

  void greet();
};
