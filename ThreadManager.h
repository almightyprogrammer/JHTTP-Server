//
// Created by humblefarmer on 04/09/2025.
//

#ifndef TCP_IMPLEMENTATION_THREADMANAGER_H
#define TCP_IMPLEMENTATION_THREADMANAGER_H
#include <atomic>

class ThreadManager {
private:
    std::atomic<int> thread_count{0};
public:
    void increment();
    void decrement();
    int get_count();
};


#endif //TCP_IMPLEMENTATION_THREADMANAGER_H