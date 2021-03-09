#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <sstream>
using namespace std;

struct Flower
{
  virtual string str() = 0;
};

struct Rose : Flower
{
  string str() override {
    return "A rose";
  }
};

/**
 * 조건문으로 각각의 경우에 대한 출력메세지 구함.
 */
struct RedFlower : Flower
{
  Flower& flower;

  RedFlower(Flower &flower) : flower(flower) {}

  string str() override
  {
    string s = flower.str();
    if (s.find("red") != string::npos) return s;
    else if (s.find("blue") != string::npos)
    {
      return s + " and red";
    }
    else return s + " that is red";
  }
};

struct BlueFlower : Flower
{
  Flower& flower;

  BlueFlower(Flower &flower) : flower(flower) {}

  string str() override
  {
    string s = flower.str();
    if (s.find("blue") != string::npos) return s;
    else if (s.find("red") != string::npos)
    {
      return s + " and blue";
    }
    else return s + " that is blue";
  }
};
