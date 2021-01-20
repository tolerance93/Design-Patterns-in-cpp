#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <boost/lexical_cast.hpp>
using namespace std;

struct Journal
{
    string title;
    vector<string> entries;
    
    //explicit: it cannot be used for implicit conversions and copy-initialization.
    explicit Journal(const string& title) : title{title}
    {
    }
    
    void add(const string& entry);
    
    // persistence is a separate concern
    void save(const string& filename);
};

void Journal::add(const string& entry)
{
    static int count = 1;
    entries.push_back(boost::lexical_cast<string>(count++) + ": " + entry);
}

// Journal안에 save가 있으면 SRP 위반!
// 각 클래스는 단 한가지의 책임을 부여받아, 수정할 이유가 단 한가지여야 한다.
void Journal::save(const string& filename)
{
    ofstream ofs(filename);
    for (auto& s : entries)
        ofs << s << endl;
}

struct PersistanceManager
{
    static void save(const Journal& j, const string& filename)
    {
        ofstream ofs(filename);
        for (auto& s : j.entries)
            ofs << s << endl;
    }
};


int main()
{
    Journal journal{"Dear Diary"};
    journal.add("I ate a bug");
    journal.add("I cried today");
    
    PersistanceManager pm;
    pm.save(journal, "diary.txt");
    return 0;
}
