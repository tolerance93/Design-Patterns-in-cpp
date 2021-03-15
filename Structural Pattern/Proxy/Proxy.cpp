#include <iostream>
#include <string>
#include <sstream>
#include <memory>
using namespace std;

struct BankAccount
{
  virtual ~BankAccount() = default;
  virtual void deposit(int amount) = 0;
  virtual void withdraw(int amount) = 0;
};

struct CurrentAccount : BankAccount // checking
{
  explicit CurrentAccount(const int balance)
    : balance(balance)
  {
  }

  void deposit(int amount) override
  {
    balance += amount;
  }

  void withdraw(int amount) override
  {
    if (amount <= balance) balance -= amount;
  }

  friend ostream& operator<<(ostream& os, const CurrentAccount& obj)
  {
    return os << "balance: " << obj.balance;
  }

private:
  int balance;
};

struct Image
{
  virtual ~Image() = default;
  virtual void draw() = 0;
};

struct Bitmap : Image
{
  Bitmap(const string& filename)
  {
    cout << "Loading image from " << filename << endl;
  }

  void draw() override
  {
    cout << "Drawing image" << endl;
  }
};

/**
 * 객체를 생성하되 불필요하게 일찍 자원이 할당되는 것을 원하지 않는 경우
 * 실제 존재하지 않는 객체를 나타낼 수도 있기 때문에 이러한 프록시를 버추얼 프록시라 부른다.
 * Bitmap과 동일한 인터페이스를 제공하고 Bitmap의 기능을 활용하되 동작 방식만 바꾼다.
 */
struct LazyBitmap : Image
{
  LazyBitmap(const string& filename): filename(filename) {}
  ~LazyBitmap() { delete bmp; }
  void draw() override
  {
    if (!bmp)
      bmp = new Bitmap(filename);
    bmp->draw();
  }

private:
  Bitmap* bmp{nullptr};
  string filename;
};

void draw_image(Image& img)
{
  cout << "About to draw the image" << endl;
  img.draw();
  cout << "Done drawing the image" << endl;
}

void virtual_proxy()
{
  LazyBitmap img{ "pokemon.png" };
  draw_image(img); // loaded whether the bitmap is loaded or not
  draw_image(img);
}

/**
 * 기존 API의 사용 방식을 정확히 동일하게(또는 최대한 비슷하게) 유지하면서 그 내부 동작만 다르게 한다.
 */
void smart_pointers()
{
  BankAccount* a = new CurrentAccount(123);
  a->deposit(321);
  delete a;

  // << will not work if you make this a shared_ptr<BankAccount>
  auto b = make_shared<CurrentAccount>(123);

  BankAccount* actual = b.get(); // pointer's own operations on a .
  b->deposit(321); // underlying object's operations are on ->
                   // note this expression is identical to what's above
  cout << *b << endl;
  // no delete

  // see shared_ptr in file structure window
}

struct Pingable
{
  virtual ~Pingable() = default;
  virtual wstring ping(const wstring& message) = 0;
};

struct Pong : Pingable
{
  wstring ping(const wstring& message) override
  {
    return message + L" pong";
  }
};

#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams

struct RemotePong : Pingable
{
  wstring ping(const wstring& message) override
  {
    wstring result;
    http_client client(U("http://localhost:9149/"));
    uri_builder builder(U("/api/pingpong/"));
    builder.append(message);
    pplx::task<wstring> task = client.request(methods::GET, builder.to_string())
      .then([=](http_response r)
    {
      return r.extract_string();
    });
    task.wait();
    return task.get();
  }
};

void tryit(Pingable& pp)
{
  wcout << pp.ping(L"ping") << "\n";
}

void communication_proxy()
{
  Pong pp;
  for (int i = 0; i < 3; ++i)
  {
    tryit(pp);
  }
}

// ======== Property Proxy ======================
//get,set 커스터마이징이 필요없다면 굳이 프록시를 쓸 이유가 없다..
template <typename T> struct Property
{
  T value;
  Property(const T initialValue)
  {
    *this = initialValue;
  }
  operator T()
  {
      //get작업 수행
    return value;
  }
  T operator =(T newValue)
  {
      //set작업 수행
    return value = newValue;
  }
};

// ===========================================

struct Creature
{
  Property<int> strength{ 10 };
  Property<int> agility{ 5 };
};

void property_proxy()
{
  Creature creature;
  creature.agility = 20;
  cout << creature.agility << endl;
}

int main()
{
  //property_proxy();
  //smart_pointers();
  //virtual_proxy();
  communication_proxy();

  getchar();
  return 0;
}
