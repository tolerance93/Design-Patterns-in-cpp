//
//  main.cpp
//  Classic State Implementation
//
//  Created by Tolerance on 2021/04/27.
//

// 디자인 패턴의 전통적인 구현 방식을 반영하고 있지만 권장하는 방법은 아님.
// 상태가 상태를 바꾼다는 접근 방식..

/**
 * 문제점
 * 1. State가 추상타입이 아님
 * 2. State 자체적으로 상태 전이를 할 수 있도록 했음
 * 3. State::on/off 동작은 이미 그 상태에 있다는 것을 가정하고 있음.
 */
#include <iostream>

using namespace std;

class LightSwitch;

struct State
{
    virtual void on(LightSwitch* ls)
    {
        cout << "Light is already on\n";
    }
    
    virtual void off(LightSwitch* ls)
    {
        cout << "Light is already off\n";
    }
};

struct OnState: State
{
    OnState()
    {
        cout << "Light is turned on\n";
    }
    
    void off(LightSwitch* lhs) override;
};

struct OffState: State
{
    OffState()
    {
        cout << "Light is turned off\n";
    }
    
    void on(LightSwitch* lhs) override;
};



class LightSwitch
{
    State *state;
public:
    LightSwitch()
    {
        state = new OffState();
    }
    
    void set_state(State* state)
    {
        this->state = state;
    }
    
    void on()
    {
        state->on(this);
    }
    
    void off()
    {
        state->off(this);
    }
};

void OnState::off(LightSwitch *ls)
{
    cout << "Switching light off...\n";
    ls->set_state(new OffState());
    delete this; // 해당 객체가 이미 생성되었다는 대단히 위험한 가정을 하고 있다.
};

void OffState::on(LightSwitch *ls)
{
    cout << "Switching light on...\n";
    ls->set_state(new OnState());
    delete this;
};

int main(int ac, char* av[])
{
    LightSwitch ls;
    
    ls.on();
    ls.off();
    ls.off();
    
    return 0;
}
