#pragma once

#include <string>
#include <vector>
#include <mutex>


template <typename> struct Observer;

template <typename T>
struct SaferObservable
{
    // 벡터는 쓰레드 안정성이 보장되지 않는다.
    std::vector<Observer<T>*> observers;
    // mutex대신 recursive_mutex사용. 대부분의 사람들이 추천하는 방법은 아님..
    typedef std::recursive_mutex mutex_t;
    mutex_t mtx;
public:
    void notify(T& source, const std::string& field_name)
    {
        std::scoped_lock<mutex_t> lock {mtx};
        for (auto observer : observers)
            if (observer)
                observer->field_changed(source, field_name);
    }
    
    void subscribe(Observer<T>& observer)
    {
        std::scoped_lock<mutex_t> lock {mtx};
        observers.push_back(&observer);
    }
    
    void unsubscribe(Observer<T>& observer)
    {
        // lock을 거는 대신 nullptr로 변경. 쉬운방법
//        auto it = std::find(begin(observers), end(observers), &observer);
//        if (it != end(observers))
//            *it = nullptr;
        
        // reentrancy문제 발생
        std::scoped_lock<mutex_t> lock {mtx};
        observers.erase(
          remove(observers.begin(), observers.end(), &observer),
        observers.end()
        );
    }
};
