/*
 * 한 덩어리의 복잡한 인터페이스를 목적에 따라 구분하여 나눔으로써, 인터페이스 모든 항목에 대한 구현을 강제하지 않고
 * 실제 필요한 인터페이스만 구현할 수 있도록 함.
 */


//struct IMachine
//{
//  virtual void print(Document& doc) = 0;
//  virtual void fax(Document& doc) = 0;
//  virtual void scan(Document& doc) = 0;
//};
//
//struct MFP : IMachine
//{
//  void print(Document& doc) override;
//  void fax(Document& doc) override;
//  void scan(Document& doc) override;
//};

#include <vector>
#include <iostream>


struct Document;

struct IPrinter
{
    virtual void print(Document& doc) = 0;
};

struct IScanner
{
    virtual void scan(Document& doc) = 0;
};

struct Printer : IPrinter
{
    void print(Document& doc) override;
};

struct Scanner : IScanner
{
    void scan(Document& doc) override;
};

//Abstract object로 생성하지 못함! pure virtual method가 있으면 abstract가 됨.
struct IMachine : IPrinter, IScanner
{
};

//Decorator pattern
struct Machine : IMachine
{
    IPrinter& printer;
    IScanner& scanner;
    
    Machine(IPrinter& printer, IScanner& scanner) : printer{printer}, scanner{scanner}
    {
    }
    
    void print(Document& doc) override
    {
        printer.print(doc);
    }
    
    void scan(Document& doc) override
    {
        scanner.scan(doc);
    }
};

