#include "Observer.h"
#include <iostream>
#include "Observable.h"
#include <boost/signals2.hpp>

using namespace std;
using namespace boost;

class Person : public Observable<Person>// observable
{
    int age;
public:
    
    Person(int age) : age(age) {}
    
    void set_age(int age)
    {
        if (this->age == age) return;
        this->age = age;
        notify(*this, "age");
    }
    
    int get_age() const
    {
        return age;
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
    
    return 0;
}
