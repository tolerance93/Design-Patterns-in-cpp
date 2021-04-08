#pragma once
#include <algorithm>

/**
 * Mediator.
 * 매개자 자체는 커뮤니케이션에 동반되는 모든 컴포넌트로부터 접근 가능해야 한다.
 * 즉, 매개자는 전역 정적 변수이거나 모든 컴포넌트에 그 참조가 노출되어야 한다.
 */

struct ChatRoom
{
  vector<Person*> people;

  void broadcast(const string& origin,
    const string& message);

  void join(Person* p);

  void message(const string& origin,
    const string& who,
    const string& message)
  {
    auto target = std::find_if(
      begin(people),
      end(people),
      [&](const Person *p) {
        return p->name == who;
      });
    if (target != end(people))
    {
      (*target)->receive(origin, message);
    }
  }
};


