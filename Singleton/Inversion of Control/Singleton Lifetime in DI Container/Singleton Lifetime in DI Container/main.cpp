#include "di.h"
#include <string>
#include <boost/lexical_cast.hpp>
#include <iostream>

using namespace std;
using namespace boost;

struct IFoo
{
    virtual string name() = 0;
};

struct Foo: IFoo
{
    static int id;
    Foo() { ++id; }
    
    string name() override {
        return "foo"s + lexical_cast<string>(id);
    }
};

int Foo::id = 0;

struct Bar
{
    std::shared_ptr<IFoo> foo;
};

int main123()
{
    /**
     * IFoo 타입 변수를 멤버로 가지는 컴포턴트가 생성될 때마다 IFoo 타입 멤버 변수를 Foo의 싱글턴 인스턴스로 초기화
     */
    auto injector = di::make_injector(
                                      di::bind<IFoo>.to<Foo>().in(di::singleton)
                                      );
    //deprecated
    auto bar1 = injector.create<std::shared_ptr<Bar>>();
    auto bar2 = injector.create<std::shared_ptr<Bar>>();
    
    cout << bar1->foo->name() << endl;
    cout << bar2->foo->name() << endl;
    
    cout << boolalpha
         << (bar1->foo.get() == bar2->foo.get())
    << endl;
    
    return 0;
}
