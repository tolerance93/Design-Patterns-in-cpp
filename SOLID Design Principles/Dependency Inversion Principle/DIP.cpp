#include <iostream>
#include <string>
#include <vector>
#include <tuple>

using namespace std;

// A. High-level modules should not depend on low-level modules.
//    Both should depend on abstractions.
// B. Abstractions should not depend on details.
//    Details should depend on abstractions.

/**
 * 이제 enum class는 형 변환이 기존보다 제약적이 되어 개발자의 실수로 발생할 버그를 방지하고자 하는 점과 기존의 enum이 가지고 있던 스코프 문제를 해결하고자 한 것이다.
 * scope를 명시해야함.
 */

enum class Relationship
{
    parent,
    child,
    sibling
};

struct Person
{
    string name;
};

struct RelationshipBrowser
{
    virtual vector<Person> find_all_children_of(const string& name) = 0;
};

/**
 * Relationships에 브라우저 기능이 있는게 맞는걸까? 단순 저장 역할만 수행하고 브라우징은 다른 곳에서 하면 어떨까?
 * Relationships가 아닌 다른곳에 브라우저 기능이 있다면?
 * Relationships를 인자로 받아 browsing을 해야할텐데 이는 Relationships에 세부 구현에 의존해야함을 의미.
 * 예를 들어, vector로 된 relations가 다른 걸로 변경된다면 browsing class도 마찬가지로 변경되어야 한다.
 * 이를 방지하려면, 인터페이스를 사용하여 원하는 데이터 타입으로 relations 를 반환하게 한 후(get_relations()), 이에 브라우징을 수행.
 */

struct Relationships : RelationshipBrowser // low-level
{
    vector<tuple<Person, Relationship, Person>> relations;
    
    void add_parent_and_child(const Person& parent, const Person& child)
    {
        relations.push_back({parent, Relationship::parent, child});
        relations.push_back({child, Relationship::child, parent});
    }
    
    /**
     * relations가 요구사항에 따라 어떻게 리턴되어야할지 인터페이스가 결정한다.
     * 즉 디테일이 추상화에 의존한다.
     */
    vector<Person> find_all_children_of(const string &name) override
    {
        vector<Person> result;
        // regardless of whether it is an lvalue or rvalue expression and I will preserve its constness.
        // rel은 rvalue이다!
        for (auto&& [first, rel, second] : relations)
        {
            if (first.name == name && rel == Relationship::parent)
            {
                result.push_back(second);
            }
        }
        return result;
    }
};

struct Research //high-level
{
    Research(RelationshipBrowser& browser)
    {
        /**
         * interface를 통해 리턴타입이 무엇인지 알고있음. 실구현 타입에 의존하지 않고 인터페이스에 의존.
         * RelationshipBrowser를 implement한 클래스가 relations를 어떻게 가지고 있는지 상관하지 않음. 디테일 의존  X
         */
        for (auto& child : browser.find_all_children_of("John"))
        {
            cout << "John has a child called " << child.name << endl;
        }
    }
    
    /**
     * (1) Relationships의 실구현 타입에 의존하여 Research 수행.
     * (2) relations가 tuple vector를 사용하고 있다는 detail에 의존함.
     */
//  Research(const Relationships& relationships)
//  {
//    auto& relations = relationships.relations;
//    for (auto&& [first, rel, second] : relations)
//    {
//      if (first.name == "John" && rel == Relationship::parent)
//      {
//        cout << "John has a child called " << second.name << endl;
//      }
//    }
//  }
};



int main()
{
  Person parent{"John"};
  Person child1{"Chris"};
  Person child2{"Matt"};

  Relationships relationships;
  relationships.add_parent_and_child(parent, child1);
  relationships.add_parent_and_child(parent, child2);

  Research _(relationships);

  return 0;
}
