#include "rw_lock/atomic_rw_lock.h"

#include <iostream>

void test_atomic_rw_lock() {
    AtomicRWLock my_rwlock;
    std::thread thread1([&my_rwlock](){
        my_rwlock.wLock();
        std::cout << std::this_thread::get_id() << " get write lock\n";
        my_rwlock.wUnLock();
    });
    std::thread thread0([&my_rwlock](){
        my_rwlock.rLock();
        std::cout << std::this_thread::get_id() << " get read lock\n";
        my_rwlock.rUnlock();
    });
    thread1.join();
    thread0.join();
}

int main(int argc, char const *argv[])
{
    test_atomic_rw_lock();
    return 0;
}
