#include <iostream>
#include <vector>

using namespace std;

struct IParticipant {
    virtual ~IParticipant() = default;
    virtual void say(int value) = 0;
    virtual void addValue(int value) = 0;
};

struct Participant;

struct Mediator {
    vector<IParticipant*> participants;
    void broadcast(IParticipant* origin, int value)
    {
        for (auto* participant : participants)
        {
            if (participant == origin) continue;
            participant->addValue(value);
        }
    }
};

struct Participant : IParticipant
{
    int value{0};
    Mediator& mediator;

    Participant(Mediator &mediator) : mediator(mediator)
    {
      mediator.participants.push_back(this);
    }

    void say(int value) override
    {
        // todo
        mediator.broadcast(this, value);
    }
    
    void addValue(int value) override
    {
        this->value += value;
    }
};

int main()
{
    Mediator mediator;
    Participant parti1{ mediator };
    Participant parti2{ mediator };
    parti1.say(2);
    cout << parti1.value << endl;
    cout << parti2.value << endl;
    parti2.say(4);
    cout << parti1.value << endl;
    cout << parti2.value << endl;
}
