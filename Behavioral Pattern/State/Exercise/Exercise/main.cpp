#include <iostream>
#include <vector>
#include <string>
using namespace std;

class CombinationLock
{
    vector<int> combination;
    vector<int> input;
public:
    string status;

    CombinationLock(const vector<int> &combination) : combination(combination) {
        status = "LOCKED";
    }

    void enter_digit(int digit)
    {
      // TODO
        input.push_back(digit);
        if (input.size() == 1)
        {
            status = to_string(digit);
        }
        else if (input.size() > 1)
        {
            status.append(to_string(digit));
        }
        
        if (input.size() == combination.size())
        {
            for (int i = 0; i < input.size(); i++)
            {
                if (input[i] != combination[i])
                {
                    status = "ERROR";
                    return;
                }
            }
            status = "OPEN";
        }
    }
};

int main()
{
    CombinationLock cl({1,2,3});
    cout << cl.status << endl;
    cl.enter_digit(1);
    cout << cl.status << endl;
    cl.enter_digit(2);
    cout << cl.status << endl;
    cl.enter_digit(5);
    cout << cl.status << endl;
}
