#include <iostream>
#include <string>
#include <vector>
#include <memory>
using namespace std;

/**
 * 불변 속성을 가진다. 저장된 잔고 값이 나중에 변경될 수 있다면 존재한 적 없는 과거 상태로 되돌리는 게 가능해진다.
 */
class Memento
{
  int balance;
public:
  Memento(int balance)
    : balance(balance)
  {
  }
    //balance필드 변수에 접근하기 위해 프렌드로 선언
  friend class BankAccount;
  friend class BankAccount2;
};

class BankAccount
{
  int balance = 0;
public:
  explicit BankAccount(const int balance)
    : balance(balance)
  {
  }

  Memento deposit(int amount)
  {
    balance += amount;
    return { balance };
  }

  void restore(const Memento& m)
  {
      //Memento에서 friend로 선언하지 않으면 메멘토의 balance에 접근할 수 없다.
    balance = m.balance;
  }

  friend ostream& operator<<(ostream& os, const BankAccount& obj)
  {
    return os << "balance: " << obj.balance;
  }
};

class BankAccount2 // supports undo/redo
{
  int balance = 0;
  vector<shared_ptr<Memento>> changes;
  int current;
public:
  explicit BankAccount2(const int balance)
  : balance(balance)
  {
    changes.emplace_back(make_shared<Memento>(balance));
    current = 0;
  }

  shared_ptr<Memento> deposit(int amount)
  {
    balance += amount;
    auto m = make_shared<Memento>(balance);
    changes.push_back(m);
    ++current;
    return m;
  }

  void restore(const shared_ptr<Memento>& m)
  {
      // 아무것도 하지 않는 기능을 제공하기위해서 m null check
      // 복구로 인한 작업도 리스트에 추가. 복구도 Undo/Redo가능
    if (m)
    {
      balance = m->balance;
      changes.push_back(m);
      current = changes.size() - 1;
    }
  }

  shared_ptr<Memento> undo()
  {
    if (current > 0)
    {
      --current;
      auto m = changes[current];
      balance = m->balance;
      return m;
    }
    return{}; // 공백 포인터를 리턴. 아무것도 하지 ㅇ낳음
  }

  shared_ptr<Memento> redo()
  {
    if (current + 1 < changes.size())
    {
      ++current;
      auto m = changes[current];
      balance = m->balance;
      return m;
    }
    return{};
  }

  friend ostream& operator<<(ostream& os, const BankAccount2& obj)
  {
    return os << "balance: " << obj.balance;
  }
};

void memento()
{
  BankAccount ba{ 100 };
  auto m1 = ba.deposit(50); // 150
  auto m2 = ba.deposit(25); // 175
  cout << ba << "\n";

  // undo to m1
  ba.restore(m1);
  cout << ba << "\n";

  // redo
  ba.restore(m2);
  cout << ba << "\n";
}

void undo_redo()
{
  BankAccount2 ba{ 100 };
  ba.deposit(50);
  ba.deposit(25); // 125
  cout << ba << "\n";

  ba.undo();
  cout << "Undo 1: " << ba << "\n";
  ba.undo();
  cout << "Undo 2: " << ba << "\n";
  ba.redo();
  cout << "Redo 2: " << ba << "\n";

  ba.undo();
}

int main()
{
  // memento()
  undo_redo();

  getchar();
  return 0;
}
