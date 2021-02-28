#include "Headers.hpp"

/**
 * 컴포지트 디자인 패턴은 보통 전체 클래스를 대상으로 적용된다.
 * 여기서는 클래스 속성 수준에서 어떻게 적용되는지 간단히 볼 것.
 */

//class Creature
//{
//  int strength, agility, intelligence;
//  // new property
//  int charisma;
//public:
//  int get_strength() const
//  {
//    return strength;
//  }
//
//  void set_strength(int strength)
//  {
//    Creature::strength=strength;
//  }
//
//  int get_agility() const
//  {
//    return agility;
//  }
//
//  void set_agility(int agility)
//  {
//    Creature::agility=agility;
//  }
//
//  int get_intelligence() const
//  {
//    return intelligence;
//  }
//
//  void set_intelligence(int intelligence)
//  {
//    Creature::intelligence=intelligence;
//  }
//
//  // sum, average, max.
//
//  double sum() const {
//    return strength + agility + intelligence;
//  }
//
//  double average() const {
//    return sum() / 3.0;
//  }
//
//  int max() const {
//    return ::max(::max(strength, agility), intelligence);
//  }
//};

class Creature
{
    /**
     * count트릭. enum에 몇개가 들어있는지!
     * enum class가 아님을 유심히 볼 것.
     */
  enum Abilities { str, agl, intl, count };
  array<int, count> abilities;
public:
  int get_strength() const { return abilities[str]; }
  void set_strength(int value) { abilities[str] = value; }

  int get_agility() const { return abilities[agl]; }
  void set_agility(int value) { abilities[agl] = value; }

  int get_intelligence() const { return abilities[intl]; }
  void set_intelligence(int value) { abilities[intl] = value; }

  int sum() const {
    return accumulate(abilities.begin(), abilities.end(), 0);
  }

  double average() const {
    return sum() / (double)count;
  }

  int max() const {
    return *max_element(abilities.begin(), abilities.end());
  }
};

int main(int ac, char* av[])
{
  Creature orc;
  orc.set_strength(16);
  orc.set_agility(11);
  orc.set_intelligence(9);

  cout << "The orc has an average stat of "
       << orc.average()
       << " and a maximum stat of "
       << orc.max()
       << "\n";

  return 0;
}
