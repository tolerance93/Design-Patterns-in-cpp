#include <iostream>
#include <string>
using namespace std;
#include <boost/signals2.hpp>
using namespace boost::signals2;

/**
 * 커맨드 디자인 패턴. 크리처에 가해진 모든 변경 작업이 모두 완료되어 결괏값이 확정된 이후에 읽어야 한다.
 * 어떤 크리처의 특정 상태 값에 대한 조회라는 개념을 캡슐화하고 있다.
 */
struct Query
{
  string creature_name;
  enum Argument { attack, defense } argument;
  int result;


  Query(const string& creature_name, const Argument argument, const int result)
    : creature_name(creature_name),
      argument(argument),
      result(result)
  {
  }
};
/**
 * [이벤트 브로커]
 * 중앙 집중화된 컴포넌트. 게임에서 발생할 수 있는 모든 변경 작업의 목록을 관리하고 특정 크리처의 공격력 또는 방어력의 상태를
 * 그간의 변경 작업 이력이 모두 반영된 상태로 구할 수 있게 한다.
 */
struct Game // mediator & observer
{
  signal<void(Query&)> queries; // 부스트 라이브러리. 어떤 신호를 발생시키고 그 신호를 기다리고 있는 몯느 수신처가 신호를 처리할 수 있게 한다.
};

class Creature
{
  Game& game;
  int attack, defense;
public:
  string name;
  Creature(Game& game, const string& name, const int attack, const int defense)
    : game(game),
      attack(attack),
      defense(defense),
      name(name)
  {
  }
  
  // no need for this to be virtual
  int GetAttack() const
  {
    Query q{ name, Query::Argument::attack, attack };
    game.queries(q);
    return q.result;
  }

  friend ostream& operator<<(ostream& os, const Creature& obj)
  {
    return os
      << "name: " << obj.name
      << " attack: " << obj.GetAttack() // note here
      << " defense: " << obj.defense;
  }
};

class CreatureModifier
{
  Game& game;
  Creature& creature;
public:
  virtual ~CreatureModifier() = default;

  // there is no handle() function

  CreatureModifier(Game& game, Creature& creature)
    : game(game),
      creature(creature)
  {
  }
};

class DoubleAttackModifier : public CreatureModifier
{
    /**
     * 나중에 객체가 소멸되었을 때 이벤트의 연결을 해제할 수 있도록 연결 정보도 저장해야 한다.
     * 변경 작업이 임시적으로 적용되고 유효한 조건을 벗어났을 때 더 이상 적용되지 않게 할 수 있다.
     */
  connection conn;
public:
  DoubleAttackModifier(Game& game, Creature& creature)
    : CreatureModifier(game, creature)
  {
    // whenever someone wants this creature's attack,
    // we return DOUBLE the value
    conn = game.queries.connect([&](Query& q)
    {
      if (q.creature_name == creature.name && 
        q.argument == Query::Argument::attack)
        q.result *= 2;
    });
  }

  ~DoubleAttackModifier()
  {
    conn.disconnect();
  }
};

// similar idea, but Query instead of Command
int main(int ac, char* av)
{
  Game game;
  Creature goblin{ game, "Strong Goblin", 2, 2 };

  cout << goblin << endl;

  {
    DoubleAttackModifier dam{ game, goblin };

    cout << goblin << endl;
  }

  cout << goblin << endl;

  getchar();
  return 0;
}
