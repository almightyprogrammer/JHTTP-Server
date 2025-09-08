//
// Created by humblefarmer on 04/09/2025.
//

#include "ThreadManager.h"



void ThreadManager::increment() {
    ++thread_count;
}

void ThreadManager::decrement() {
    --thread_count;
}

int ThreadManager::get_count() {
    return thread_count;
}