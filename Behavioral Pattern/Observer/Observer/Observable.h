#pragma once

#include <string>
#include <vector>

template <typename> struct Observer;


/**
 * Person의 변경을 모니터링 하려는 관찰자들을 private리스트로 관리한다.
 * 관찰자가 Person의 변경 이벤트에 수신 등록 또는 해제할 수 있게 한다.
 * notify()를 통해 변경 이벤트가 발생했을 때 모든 관찰자에게 정보가 전달 되도록 한다.
 */
template <typename T>
struct Observable
{
    std::vector<Observer<T>*> observers;
public:
    void notify(T& source, const std::string& field_name)
    {
        for (auto observer : observers)
            observer->field_changed(source, field_name);
    }
    
    void subscribe(Observer<T>& observer)
    {
        observers.push_back(&observer);
    }
    
    void unsubscribe(Observer<T>& observer)
    {
        observers.erase(
          remove(observers.begin(), observers.end(), &observer),
        observers.end()
        );
    }
};
