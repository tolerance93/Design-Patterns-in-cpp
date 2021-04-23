#include "Observer.h"
#include <iostream>
#include "Observable.h"
#include <boost/signals2.hpp>
#include "SaferObservable.h"

using namespace std;
using namespace boost;

class Person : public SaferObservable<Person>// observable
{
    int age;
public:
    Person() {}
    Person(int age) : age(age) {}
    
    //종속성이 생길 경우 유지보수가 대단히 어려워짐
    void set_age(int age)
    {
        if (this->age == age) return;
        
        auto old_can_vote = get_can_vote();
        this->age = age;
        notify(*this, "age");
        
        if (old_can_vote != get_can_vote())
            notify(*this, "can_vote");
    }
    
    int get_age() const
    {
        return age;
    }
    //setter가 없을 경우 notify()가 여기에 디펜던시를 가짐. 많아질 경우 매우 복잡한 디펜던시 관계가 생길 수 있음.
    // 현재 can_vote가 age에 종속성을 가지는 상황
    bool get_can_vote() const {
        return age >= 16;
    }
};

//observer & observable
struct ConsolePersonObserver : public Observer<Person>
{
private:
    void field_changed(Person &source, const std::string &field_name) override
    {
        cout << "Person's " << field_name << " has changed to ";
        if (field_name == "age") cout << source.get_age();
        if (field_name == "can_vote")
            cout << boolalpha << source.get_can_vote();
        cout << "\n";
    }
};

template <typename T> struct Observable2
{
    signals2::signal<void(T&, const string&)> field_changed;
};

class Person2 : public Observable2<Person2>
{
    int age;
public:
    int get_age() const
    {
        return age;
    }
    
    void set_age(int age)
    {
        if (this->age == age) return;
        this->age = age;
        field_changed(*this, "age");
    }
};

#include <iostream>

struct TrafficAdministration : Observer<Person>
{
    void field_changed(Person &source, const std::string &field_name) override
    {
        if (field_name == "age")
        {
            if (source.get_age() < 17)
            {
                cout << "whoa there, you're not old enough to drive!\n";
            }
            else
            {
                cout << "Oh, ok. we no longer care!\n";
                source.unsubscribe(*this);
            }
        }
    }
    
};

int main(int argc, const char * argv[]) {
    /*
    Person person{10};
    //실제 동작을 수행함: 결과 프린트
    ConsolePersonObserver cpo;
    //Observable 목록에 cpo추가
    person.subscribe(cpo);
    
    //notify()를 통해 Observable에 등록된 목록에서 Observer의 field_chaged를 부름.
    person.set_age(11);
    person.set_age(12);
    
    person.unsubscribe(cpo);
    person.set_age(13);
     */
    
    Person p;
    TrafficAdministration ta;
    p.subscribe(ta);
    
    p.set_age(15);
    p.set_age(16);
    try {
        p.set_age(17);
    } catch (const std::exception& e) {
        //not reached why?
        cout << "Oops, " << e.what() << "\n";
    }
   
    
    /*
    //boost
    Person2 p2;
    //subscribe boost signal은 옵저버가 따로 필요없다. 람다함수 이용
    auto conn = p2.field_changed.connect(
      [](Person2& p, const string& field_name)
      {
        cout << field_name << " has changed\n";
      }
    );
    p2.set_age(20);
    conn.disconnect(); // unsubscribe;
    */
    return 0;
}
