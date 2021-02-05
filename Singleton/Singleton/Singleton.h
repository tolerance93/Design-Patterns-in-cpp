#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <boost/lexical_cast.hpp>
#include <vector>

/**
 * 싱글턴 패턴
 * (1) 어떤 특정 컴포넌트의 인스턴스가 애플리케이션 전체에서 단 하나만
 * 존재해야 하는 상황을 처리하기 위해 고안되었다.
 * (2) constructor call이 값비싼 경우 <= SpenRecognizerEngine!
 * (3) Want to prevent anyone to create additional copies
 * (4) lazy instantiation, thread safety
 */

class Database
{
public:
  virtual int get_population(const std::string& name) = 0;
};

class SingletonDatabase : public Database
{
  SingletonDatabase()
  {
    std::cout << "Initializing database" << std::endl;

    std::ifstream ifs("/Users/tolerance/Desktop/DesignPattern/Singleton/Singleton/capitals.txt");

    std::string s, s2;
    while (getline(ifs, s))
    {
      getline(ifs, s2);
      int pop = boost::lexical_cast<int>(s2);
      capitals[s] = pop;
    }
    //instance_count++;
  }

  std::map<std::string, int> capitals;

public:
  //static int instance_count;
  //객체를 생성하지 못하게 하기위해 삭제
  SingletonDatabase(SingletonDatabase const&) = delete;
  void operator=(SingletonDatabase const&) = delete;

  static SingletonDatabase& get()
  {
      /**
       * static 전역 객체의 문제점은 각각의 컴파일 단위 바이너리들에서 초기화 순서가 정의되어 있지 않다.
       * 이 함수의 thread 안정성은 C++11이상 버전에서만 보장된다.
       * static 객체를 초기화하는 코드 앞뒤로 컴파일러가 락을 삽입하여 초기화 와중에
       * 동시에 다른 스레드에서 접근하는 것을 방지해 주는지 확인해야 한다.
       *
       * static 변수의 초기화는 전체 런타임 중에 단 한 번만 수행된다.
       */
    static SingletonDatabase db;
    return db;
  }

  int get_population(const std::string& name) override
  {
    return capitals[name];
  }

  /*
  static SingletonDatabase* get_instance()
  {
    if (!instance)
      instance = new SingletonDatabase;
    return instance; // atexit
  }
  */
};

//int SingletonDatabase::instance_count = 0;

class DummyDatabase : public Database
{
  std::map<std::string, int> capitals;
public:


  DummyDatabase()
  {
    capitals["alpha"] = 1;
    capitals["beta"] = 2;
    capitals["gamma"] = 3;
  }

  int get_population(const std::string& name) override {
    return capitals[name];
  }
};

/**
 * SingletonRecordFinder는 SingletonDatabase에 밀접하게 의존한다.
 * (1) 실제 데이터를 이용해서 테스트해야한다.
 */
struct SingletonRecordFinder
{
  int total_population(std::vector<std::string> names)
  {
    int result = 0;
    for (auto& name : names)
      result += SingletonDatabase::get().get_population(name);
    return result;
  }
};

struct ConfigurableRecordFinder
{
  explicit ConfigurableRecordFinder(Database& db)
    : db{db}
  {
  }

  int total_population(std::vector<std::string> names) const
  {
    int result = 0;
    for (auto& name : names)
      result += db.get_population(name);
    return result;
  }

  Database& db;
};
