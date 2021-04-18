#include "Observer.h"
#include <iostream>

using namespace std;

class Person // observable
{
    int age;
public:
    void set_age(int age)
    {
        Person::age = age;
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
        cout << "Person's " << field_name << "has changed to ";
        if (field_name == "age") cout << source.get_age();
        cout << "\n.";
    }
};

#include <iostream>

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    return 0;
}
