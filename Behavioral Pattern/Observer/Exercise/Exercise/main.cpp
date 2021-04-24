#include <iostream>
#include <vector>
#include <bits/stdc++.h>
using namespace std;

struct Rat;

struct IRat {
    virtual void notify() = 0;
    virtual void updateAttack(int val) = 0;
    virtual ~IRat() = default;
};

struct Game
{
    vector<IRat*> rats;
    
    void upateRat() {
        for (auto rat : rats)
        {
            rat->updateAttack(rats.size());
        }
    }
    
    void removeRat(Rat* rat) {
        rats.erase(
          remove(rats.begin(), rats.end(), rat),
          rats.end()
        );
    }
};



struct Rat : IRat
{
    Game& game;
    int attack{1};

    Rat(Game &game) : game(game)
    {
      // todo, obviously
        game.rats.push_back(this);
        notify();
    }
    
    void notify() override
    {
        game.upateRat();

    }
    
    void updateAttack(int val) override
    {
        attack = val;
    }

    ~Rat()
    {
        game.removeRat(this);
        notify();
    }
};
