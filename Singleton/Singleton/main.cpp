//
//  main.cpp
//  Singleton
//
//  Created by Tolerance on 2021/02/04.
//

#include <iostream>
#include "Singleton.h"
using namespace std;

int main(int argc, const char * argv[]) {
    cout << SingletonDatabase::get().get_population("Tokyo") << endl;
    return 0;
}
