#include <iostream>
#include <string>
#include <vector>
using namespace std;

/**
 * 커맨드 디자인 패턴은 어떤 객체를 활용할 때 직접 그 객체의 API를 호출하여 조작하는 대신, 작업을 어떻게 하라고 명령을 보내는 방식을 제안.
 * cf) a = 1같은 대입 구문은 추적 불가.
 */

/**
 * deposit과 withdraw를 직접 호출하지 않고 커맨드 이용
 */
struct BankAccount
{ 
  int balance = 0;
  int overdraft_limit = -500;

  void deposit(int amount)
  {
    balance += amount;
    cout << "deposited " << amount << ", balance now " << 
      balance << "\n";
  }

  void withdraw(int amount)
  {
    if (balance - amount >= overdraft_limit)
    {
      balance -= amount;
      cout << "withdrew " << amount << ", balance now " << 
        balance << "\n";
    }
  }
};

struct Command
{
  virtual ~Command() = default;
  virtual void call() const = 0;
  virtual void undo() const = 0;
};

// should really be BankAccountCommand
struct BankAccountCommand : Command
{
  BankAccount& account;
  enum Action { deposit, withdraw } action;
  int amount;

  BankAccountCommand(BankAccount& account, 
    const Action action, const int amount)
    : account(account), action(action), amount(amount) {}

    /**
     * deposit(), withdraw() 멤버 함수가 클라이언트에 노출되는 것을 피하고 싶다면, private으로 선언하고 BankAccountCommand를 friend 클래스로 선언하면 된다.
     */
  void call() const override
  {
    switch (action)
    {
    case deposit:
      account.deposit(amount);
      break;
    case withdraw: 
      account.withdraw(amount);
      break;
    default: break;
    }
  }

  void undo() const override
  {
    switch (action)
    {
    case withdraw:
      account.deposit(amount);
      break;
    case deposit:
      account.withdraw(amount);
      break;
    default: break;
    }
  }
};

// vector doesn't have virtual dtor, but who cares?
struct CompositeBankAccountCommand 
  : vector<BankAccountCommand>, Command
{
  CompositeBankAccountCommand(const initializer_list<value_type>& items)
    : vector<BankAccountCommand>(items) {}

  void call() const override
  {
    for (auto& cmd : *this)
      cmd.call();
  }

  void undo() const override
  {
    for (auto& cmd : *this)
      cmd.undo();
  }
};

int main_987()
{
  BankAccount ba;
  /*vector<BankAccountCommand> commands{*/
  CompositeBankAccountCommand commands {
    BankAccountCommand{ba, BankAccountCommand::deposit, 100},
    BankAccountCommand{ba, BankAccountCommand::withdraw, 200}
  };

  cout << ba.balance << endl;

  // apply all the commands
  /*for (auto& cmd : commands)
  {
    cmd.call();
  }*/
  commands.call();

  cout << ba.balance << endl;

  /*for_each(commands.rbegin(), commands.rend(),
    [](const BankAccountCommand& cmd) { cmd.undo(); });*/
  commands.undo();

  cout << ba.balance << endl;

  getchar();
  return 0;
}
